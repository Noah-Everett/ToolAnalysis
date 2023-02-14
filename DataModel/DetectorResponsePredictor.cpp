#include "DetectorResponsePredictor.h"
  
void DetectorResponsePredictor::reset_members()
{
    m_verbosity = m_verbosity_warning       ;
    delete m_vtxGeo                         ;
    delete m_histReader_TH2D                ;
    delete m_histReader_TH1D                ;
    m_hists_emission_initialEnergies.clear();
    delete m_hists_emission_water           ;
    delete m_hists_emission_sci             ;
    delete m_hist_trans_tankWater           ;
    delete m_hist_trans_MRDsci              ;
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
        Log_debug( "Argument `t_hists_IDs` for `load_hists_emission_tankWater()` 
                    and `load_hists_emission_MRDsci()` should match.", m_verbosity_error );
        return false;
    }
    m_histReader_TH2D = new THistReader< int, TH2D >( t_hists_paths, t_hists_IDs, t_hists_names );
    m_hists_emission = m_histReader_TH2D->get_histsMap_cp();
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

bool DetectorResponsePredictor::load_hist(       TH1D  * m_hist     ,
                                           const string& t_hist_path,
                                           const string& t_hist_name ) {
    Log_debug( "Loading histogram.", m_verbosity_debug );
    m_histReader_TH1D = new THistReader< bool, TH1D >( t_hist_path, 1, t_hist_name );
    m_hist = m_histReader_TH1D->get_histsMaps_cp()->at( 1 );
    if( m_histReader_TH1D ) {
        delete m_histReader_TH1D;
        return true;
    } else
        return false;
}

double DetectorResponsePredictor::eval_hists_emission(       map< double, TH2D* >* t_hists_emission,
                                                       const double              & t_initialEnergy ,
                                                       const double              & t_trackLength   , 
                                                       const double              & t_photonAngle   ) {
    // Bisect histogram energies to find the histograms 
    // which have energies just lower and just higher 
    // than t_initialEnergy.
    int index_lower{ 0 }, index_upper{ m_hists_emission_initialEnergies.size() - 1 };
    while( index_upper - index_lower != 1 ) {
        if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] < t_initialEnergy )
            index_lower = ( index_lower + index_upper ) / 2;
        else if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] > t_initialEnergy )
            index_upper = ( index_lower + index_upper ) / 2;
    }

    // Use linear inerpolation between upper and lower histograms
    // to find expected emission value.
    double initialEnergy_lower{ m_hists_emission_initialEnergies[ index_lower ] }, 
           initialEnergy_upper{ m_hists_emission_initialEnergies[ index_upper ] };
    double emission_lower{ m_hists_emission[ initialEnergy_lower ]->Interpolate( t_trackLength, t_initialEnergy ) },
           emission_upper{ m_hists_emission[ initialEnergy_upper ]->Interpolate( t_trackLength, t_initialEnergy ) };
    double slope{ ( emission_upper - emission_upper ) / ( initialEnergy_lower - initialEnergy_upper ) };
    double value{ slope * ( t_initialEnergy - initialEnergy_lower ) + emission_lower }; // point slope
    return value;
}


double DetectorResponsePredictor::eval_hist(       TH1D  * t_hist,
                                              const double& t_x   ) {
    return t_hist->Interpolate( t_x );
}
    
void DetectorResponsePredictor::Log_debug( string& t_message, int& t_verbosity ) {
    m_temp_string = "DetectorResponsePredictor";
    Log( m_temp_string + __FILE__ + "::" + __FUNCTION__ + " (" + __LINE__ + "): " + t_message, t_verbosity, m_verbosity )
saf
