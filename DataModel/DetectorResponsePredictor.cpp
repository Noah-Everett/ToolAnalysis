////////////////////////////////////
// DetectorResponsePrediction.cpp //
////////////////////////////////////
//
// Noah Everett (noah.everett@mines.sdsmt.edu)
// Created Febuary 13, 2023
//
// Abstract class for generalized detector 
// response prediction.  All information 
// (PDFs, simulations, etc.) needs to be 
// set by the user.
//
// Notation: t_ = function variable
//           m_ = member variable
//           naming: mainVariable_subCategory (use '_' like LaTeX)
//
////////////////////////////////////

#include "DetectorResponsePredictor.h"
  
void DetectorResponsePredictor::reset_members()
{
    m_verbosity = m_verbosity_warning       ;
    // delete m_vtxGeo                      ;
    delete m_histReader_TH2D                ;
    delete m_histReader_TH1D                ;
    m_hists_emission_initialEnergies.clear();
    delete m_hists_emission_tankWater       ;
    delete m_hists_emission_MRDsci          ;
    delete m_hist_transmission_tankWater    ;
    delete m_hist_transmission_MRDsci       ;
    delete m_hist_dEdX_tankWater            ;
    delete m_hist_dEdX_tankSteel            ;
    delete m_hist_dEdX_MRDiron              ;
    delete m_hist_dEdX_MRDiron              ;
}

bool DetectorResponsePredictor::load_hists_emission(       map   < double, TH2D* >* t_hists_emission,
                                                     const vector< string        >& t_hists_paths   , 
                                                     const vector< double        >& t_hists_IDs     ,
                                                     const vector< string        >& t_hists_names   ) {
    Log_debug( "Loading emission histograms.", m_verbosity_debug );
    if( m_hists_emission_initialEnergies.empty() ) {
        for( int i{ 0 }; i + 1 < t_hists_IDs.size(); i++ ) // check IDs are strictly increasing
            if( t_hists_IDs[ i ] >= t_hists_IDs[ i + 1 ] ) {
                Log_debug( "`t_hists_IDs` does not have strictly increasing entries.", m_verbosity_error );
                return false;
            }
        m_hists_emission_initialEnergies = t_hists_IDs;
    } else if( m_hists_emission_initialEnergies != t_hists_IDs ) {
        Log_debug( "Argument `t_hists_IDs` for `load_hists_emission_tankWater()`" 
                   " and `load_hists_emission_MRDsci()` should match.", m_verbosity_error );
        return false;
    }
    m_histReader_TH2D = new THistReader< double, TH2D >( t_hists_paths, t_hists_IDs, t_hists_names );
    t_hists_emission = m_histReader_TH2D->get_histsMap_cp();
    if( m_histReader_TH2D ) {
        delete m_histReader_TH2D;
        return true;
    } else
        return false;
}

inline bool DetectorResponsePredictor::load_hists_emission_tankWater( const vector< string >& t_hists_paths, 
                                                                      const vector< double >& t_hists_IDs  ,
                                                                      const vector< string >& t_hists_names ) {
    Log_debug( "Loading tank water emission histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_tankWater, t_hists_paths, t_hists_IDs, t_hists_names );
}

inline bool DetectorResponsePredictor::load_hists_emission_MRDsci( const vector< string >& t_hists_paths, 
                                                                   const vector< double >& t_hists_IDs  ,
                                                                   const vector< string >& t_hists_names ) {
    Log_debug( "Loading MRD scintilator emission histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_MRDsci, t_hists_paths, t_hists_IDs, t_hists_names );
}

bool DetectorResponsePredictor::load_hist(       TH1D  *  m_hist     ,
                                           const string& t_hist_path,
                                           const string& t_hist_name ) {
    Log_debug( "    Loading histogram.", m_verbosity_debug );
    m_histReader_TH1D = new THistReader< bool, TH1D >( { t_hist_path }, { 1 }, { t_hist_name } );
    m_hist = m_histReader_TH1D->get_histsMap_cp()->at( true );
    if( m_histReader_TH1D ) {
        delete m_histReader_TH1D;
        Log_debug( "    Successfully loaded histogram.", m_verbosity_debug );
        return true;
    } else
        Log_debug( "    Failed to load histogram.", m_verbosity_debug );
        return false;
}

inline bool DetectorResponsePredictor::load_hist_transmission_tankWater( const string& t_hist_path,
                                                                         const string& t_hist_name ) {
    Log_debug( "Loading tank water transmission histogram.", m_verbosity_debug );
    return load_hist( m_hist_transmission_tankWater, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_transmission_MRDsci( const string& t_hist_path,
                                                                      const string& t_hist_name ) {
    Log_debug( "Loading MRD scintilator transmission histogram.", m_verbosity_debug );
    return load_hist( m_hist_transmission_MRDsci, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_tankWater( const string& t_hist_path,
                                                                 const string& t_hist_name ) {
    Log_debug( "Loading tank water dEdX histogram.", m_verbosity_debug );
    return load_hist( m_hist_dEdX_tankWater, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_tankSteel( const string& t_hist_path,
                                                                 const string& t_hist_name ) {
    Log_debug( "Loading tank steel dEdX histogram.", m_verbosity_debug );
    return load_hist( m_hist_dEdX_tankSteel, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_MRDsci( const string& t_hist_path,
                                                              const string& t_hist_name ) {
    Log_debug( "Loading MRD scintilator dEdX histogram.", m_verbosity_debug );
    return load_hist( m_hist_dEdX_MRDsci, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_MRDiron( const string& t_hist_path,
                                                               const string& t_hist_name ) {
    Log_debug( "Loading MRD iron dEdX histogram.", m_verbosity_debug );
    return load_hist( m_hist_dEdX_MRDiron, t_hist_path, t_hist_name );
}

double DetectorResponsePredictor::eval_hists_emission(       map< double, TH2D* >* t_hists_emission,
                                                       const double              & t_initialEnergy ,
                                                       const double              & t_trackLength   , 
                                                       const double              & t_photonAngle   ) const {
    Log_debug( "    Evaluating emission histograms.", m_verbosity_debug );
    // Bisect histogram energies to find the histograms 
    // which have energies just lower and just higher 
    // than t_initialEnergy.
    int index_lower{ 0 }, index_upper{ m_hists_emission_initialEnergies.size() - 1 };
    Log_debug( "    Finding histograms with initial primary energies just below and above `t_initialEnergy` with bisection.", m_verbosity_debug );
    while( index_upper - index_lower != 1 ) {
        m_temp_string = "        [|]-Lower: index=";
        Log_debug( m_temp_string + to_string( index_lower ) + " && initEnergy=" + to_string( m_hists_emission_initialEnergies[ index_lower ] ) + "\n"
                   "         \\--Upper: index=" + to_string( index_upper ) + " && initEnergy=" + to_string( m_hists_emission_initialEnergies[ index_upper ] ), m_verbosity_debug );
        // Log_debug( m_temp_string + to_string( index_lower ) + " && initEnergy=", m_verbosity_debug );
        if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] < t_initialEnergy )
            index_lower = ( index_lower + index_upper ) / 2;
        else if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] > t_initialEnergy )
            index_upper = ( index_lower + index_upper ) / 2;
    }

    // Use linear inerpolation between upper and lower histograms
    // to find expected emission value.
    double initialEnergy_lower{ m_hists_emission_initialEnergies[ index_lower ] };
    double initialEnergy_upper{ m_hists_emission_initialEnergies[ index_upper ] };
    double emission_lower{ t_hists_emission->at( initialEnergy_lower )->Interpolate( t_trackLength, t_initialEnergy ) };
    double emission_upper{ t_hists_emission->at( initialEnergy_upper )->Interpolate( t_trackLength, t_initialEnergy ) };
    double slope{ ( emission_upper - emission_upper ) / ( initialEnergy_lower - initialEnergy_upper ) };
    double value{ slope * ( t_initialEnergy - initialEnergy_lower ) + emission_lower }; // point slope
    Log_debug( "    Interpolated value=" + to_string( value ) + ".", m_verbosity_debug );
    return value;
}
 
inline double DetectorResponsePredictor::eval_hists_emission_tankWater( const double& t_initialEnergy ,
                                                                        const double& t_trackLength   , 
                                                                        const double& t_photonAngle   ) const {
    Log_debug( "Evaluating tank water emission histograms.", m_verbosity_debug );
    return eval_hists_emission( m_hists_emission_tankWater, t_initialEnergy, t_trackLength, t_photonAngle );
}

inline double DetectorResponsePredictor::eval_hists_emission_MRDsci( const double& t_initialEnergy ,
                                                                     const double& t_trackLength   , 
                                                                     const double& t_photonAngle   ) const {
    Log_debug( "Evaluating MRD scintilator emission histograms.", m_verbosity_debug );
    return eval_hists_emission( m_hists_emission_MRDsci, t_initialEnergy, t_trackLength, t_photonAngle );
}

double DetectorResponsePredictor::eval_hist(       TH1D   * t_hist,
                                             const double & t_x   ) const {
    Log_debug( "    Evaluating histogram", m_verbosity_debug );
    return t_hist->Interpolate( t_x );
}

inline double DetectorResponsePredictor::eval_hist_transmission_tankWater( const double& t_photonEnergy  ) const {
    Log_debug( "Evaluating tank water transmission histogram.", m_verbosity_debug );
    return eval_hist( m_hist_transmission_tankWater, t_photonEnergy );
}

inline double DetectorResponsePredictor::eval_hist_transmission_MRDsci( const double& t_photonEnergy  ) const {
    Log_debug( "Evaluating MRD scintilator transmission histogram.", m_verbosity_debug );
    return eval_hist( m_hist_transmission_tankWater, t_photonEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_tankWater( const double& t_primaryEnergy ) const {
    Log_debug( "Evaluating MRD scintilator dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_tankWater, t_primaryEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_tankSteel( const double& t_primaryEnergy ) const {
    Log_debug( "Evaluating tank steel dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_tankSteel, t_primaryEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_MRDsci( const double& t_primaryEnergy ) const {
    Log_debug( "Evaluating MRD scinitilator dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_MRDsci, t_primaryEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_MRDiron( const double& t_primaryEnergy ) const {
    Log_debug( "Evaluating MRD ironr dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_MRDiron, t_primaryEnergy );
}
    
inline void DetectorResponsePredictor::Log_debug( const string& t_message, const int& t_verbosity ) const {
    if( t_verbosity >= m_verbosity )
        cout << "DetectorResponsePredictor || " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message;
}

inline void DetectorResponsePredictor::Log_debug( const string&& t_message, const int& t_verbosity ) const {
    if( t_verbosity >= m_verbosity )
        cout << "DetectorResponsePredictor || " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message;
}