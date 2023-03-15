//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                               DetectorResponsePredictor.cpp                             //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*// Author:                                                                                 //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                                           //*//
//*//   Created Febuary 13, 2023                                                              //*//
//*//                                                                                         //*//
//*// About:                                                                                  //*//
//*//   This file contains a class for generalized detector response prediction. All          //*//
//*//   information (PDFs, histograms, etc.) needs to be set by the user.                     //*//
//*//                                                                                         //*//
//*// Notation:                                                                               //*//
//*//   t_ = function argument                                                                //*//
//*//   m_ = member variable                                                                  //*//
//*//   mainVariable_subCategory (use '_' like LaTeX)                                         //*//
//*//                                                                                         //*//
//*// Units:                                                                                  //*//
//*//   Energy        [MeV]                                                                   //*//
//*//   Distance      [m]                                                                     //*//
//*//   Angle         [rad]                                                                   //*//
//*//   Time          [s]                                                                     //*//
//*//   Transmittance [1/m] (%/m)                                                             //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

#include "DetectorResponsePredictor.h"
  
void DetectorResponsePredictor::reset_members()
{
    m_verbosity = m_verbosity_warning       ;
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

bool DetectorResponsePredictor::load_hists_emission(       map   < int, TH2D* >* t_hists_emission,
                                                     const vector< string     >& t_hists_paths   , 
                                                     const vector< int        >& t_hists_IDs     ,
                                                     const vector< string     >& t_hists_names   ,
                                                           double              & t_binWidth_s    ,
                                                           double              & t_binWidth_theta,
                                                           double              & t_binWidth_phi   ) {
    Log_debug( "Loading emission histograms.", m_verbosity_debug );
    
    // Check IDs are strictly increasing
    if( m_hists_emission_initialEnergies.empty() ) {
        for( int i{ 0 }; i + 1 < t_hists_IDs.size(); i++ ) 
            if( t_hists_IDs[ i ] >= t_hists_IDs[ i + 1 ] ) {
                Log_debug( "`t_hists_IDs` does not have strictly increasing entries.", m_verbosity_error );
                return false;
            }
        m_hists_emission_initialEnergies = t_hists_IDs;
    }
    // Check IDs match existing IDs
    else if( m_hists_emission_initialEnergies != t_hists_IDs ) {
        Log_debug( "Argument `t_hists_IDs` for `load_hists_emission_tankWater()`" 
                   " and `load_hists_emission_MRDsci()` should match.", m_verbosity_error );
        return false;
    }

    // Load histogram map
    THistReader< int, TH2D >* histReader{ new THistReader< int, TH2D >( t_hists_paths, t_hists_IDs, t_hists_names ) };
    if( t_hists_emission ) delete t_hists_emission;
    *t_hists_emission = *( histReader->get_histsMap_cp() );
    delete histReader;
    
    // If map wasnt returned
    if( !t_hists_emission ) {
        Log_debug( "`t_hists_emission` wasnt set.", m_verbosity_error );
        return false;
    }

    // Check that a hist with 0 energy exists
    if( t_hists_emission->find( 0 ) == t_hists_emission->end() ) {
        Log_debug( "`t_hists_emission` doesnt contain a hist with zero energy.", m_verbosity_error );
        return false;
    }
    
    // Check all TH2D axis are the same
    double min_x  { t_hists_emission->at( 0 )->GetXaxis()->GetXmin()  };
    double max_x  { t_hists_emission->at( 0 )->GetXaxis()->GetXmax()  };
    int    nBins_x{ t_hists_emission->at( 0 )->GetXaxis()->GetNbins() };
    double min_y  { t_hists_emission->at( 0 )->GetYaxis()->GetXmin()  };
    double max_y  { t_hists_emission->at( 0 )->GetYaxis()->GetXmax()  };
    int    nBins_y{ t_hists_emission->at( 0 )->GetYaxis()->GetNbins() };
    for( auto const& [ i, h ] : *t_hists_emission )
        if( h->GetXaxis()->GetXmin()  != min_x   ||
            h->GetXaxis()->GetXmax()  != max_x   ||
            h->GetXaxis()->GetNbins() != nBins_x ||
            h->GetYaxis()->GetXmin()  != min_y   ||
            h->GetYaxis()->GetXmax()  != max_y   ||
            h->GetYaxis()->GetNbins() != nBins_y   ) {
            Log_debug( "All histograms in `t_hists_emission` do not have the same axes (same min, max, and bin counts).", m_verbosity_error );
            return false;
        }
    
    // Set bin widths
    t_binWidth_s     = t_hists_emission->at( 0 )->GetXaxis()->GetBinWidth( 0 );
    t_binWidth_theta = t_hists_emission->at( 0 )->GetYaxis()->GetBinWidth( 0 );
    t_binWidth_phi   = t_hists_emission->at( 0 )->GetYaxis()->GetBinWidth( 0 );

    return true;
}

inline bool DetectorResponsePredictor::load_hists_emission_tankWater( const vector< string >& t_hists_paths, 
                                                                      const vector< int    >& t_hists_IDs  ,
                                                                      const vector< string >& t_hists_names ) {
    Log_debug( "Loading tank water emission histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_tankWater, t_hists_paths             , 
                                t_hists_IDs               , t_hists_names             , 
                                m_binWidth_s_tankWater    , m_binWidth_theta_tankWater, 
                                m_binWidth_phi_tankWater                                  );
}

inline bool DetectorResponsePredictor::load_hists_emission_MRDsci( const vector< string >& t_hists_paths, 
                                                                   const vector< int    >& t_hists_IDs  ,
                                                                   const vector< string >& t_hists_names ) {
    Log_debug( "Loading MRD scintilator emission histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_MRDsci, t_hists_paths          , 
                                t_hists_IDs            , t_hists_names          ,
                                m_binWidth_s_MRDsci    , m_binWidth_theta_MRDsci,
                                m_binWidth_phi_MRDsci                            );
}

inline bool DetectorResponsePredictor::load_hists_emission_tankWater_energies( const vector< string >& t_hists_paths, 
                                                                               const vector< int    >& t_hists_IDs  ,
                                                                               const vector< string >& t_hists_names ) {
    Log_debug( "Loading tank water average emission energy histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_tankWater_energies, t_hists_paths             , 
                                t_hists_IDs                        , t_hists_names             , 
                                m_binWidth_s_tankWater             , m_binWidth_theta_tankWater, 
                                m_binWidth_phi_tankWater                                       );
}

inline bool DetectorResponsePredictor::load_hists_emission_MRDsci_energies( const vector< string >& t_hists_paths, 
                                                                            const vector< int    >& t_hists_IDs  ,
                                                                            const vector< string >& t_hists_names ) {
    Log_debug( "Loading MRD scintilator average emission energy histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_MRDsci_energies, t_hists_paths          , 
                                t_hists_IDs                     , t_hists_names          ,
                                m_binWidth_s_MRDsci             , m_binWidth_theta_MRDsci,
                                m_binWidth_phi_MRDsci                                     );
}

template< typename type_hist >
bool DetectorResponsePredictor::load_hist( const type_hist*  m_hist    ,
                                           const string   & t_hist_path,
                                           const string   & t_hist_name ) {
    Log_debug( "    Loading histogram.", m_verbosity_debug );
    THistReader< bool, type_hist >* histReader{ new THistReader< bool, type_hist >( { t_hist_path }, { 1 }, { t_hist_name } ) };
    m_hist = histReader->get_histsMap_cp()->at( true );
    if( histReader ) {
        delete histReader;
        Log_debug( "    Successfully loaded histogram.", m_verbosity_debug );
        return true;
    } else
        Log_debug( "    Failed to load histogram.", m_verbosity_debug );
        return false;
}

inline bool DetectorResponsePredictor::load_hist_transmission_tankWater( const string& t_hist_path,
                                                                         const string& t_hist_name ) {
    Log_debug( "Loading tank water transmission histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_transmission_tankWater, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_transmission_MRDsci( const string& t_hist_path,
                                                                      const string& t_hist_name ) {
    Log_debug( "Loading MRD scintilator transmission histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_transmission_MRDsci, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_tankWater( const string& t_hist_path,
                                                                 const string& t_hist_name ) {
    Log_debug( "Loading tank water dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_tankWater, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_tankSteel( const string& t_hist_path,
                                                                 const string& t_hist_name ) {
    Log_debug( "Loading tank steel dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_tankSteel, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_MRDsci( const string& t_hist_path,
                                                              const string& t_hist_name ) {
    Log_debug( "Loading MRD scintilator dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_MRDsci, t_hist_path, t_hist_name );
}

inline bool DetectorResponsePredictor::load_hist_dEdX_MRDiron( const string& t_hist_path,
                                                               const string& t_hist_name ) {
    Log_debug( "Loading MRD iron dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_MRDiron, t_hist_path, t_hist_name );
}

pair< int, int > DetectorResponsePredictor::get_closestEmissionHists( const map< int, TH2D* >* t_hists_emission,
                                                                      const double             t_initialEnergy  ) const {
    // Bisect histogram energies to find the histograms 
    // which have energies just lower and just higher 
    // than t_initialEnergy.
    int index_lower{ 0 }, index_upper{ m_hists_emission_initialEnergies.size() - 1 };
    Log_debug( "    Finding histograms with initial primary energies just below and above `t_initialEnergy` with bisection.", m_verbosity_debug );
    while( index_upper - index_lower != 1 ) {
        Log_debug( "        [|]-Lower: index="  + to_string( index_lower ) + " && initEnergy=" + to_string( m_hists_emission_initialEnergies[ index_lower ] ) + "\n"
                   "         \\--Upper: index=" + to_string( index_upper ) + " && initEnergy=" + to_string( m_hists_emission_initialEnergies[ index_upper ] ), m_verbosity_debug );
        // Log_debug( m_temp_string + to_string( index_lower ) + " && initEnergy=", m_verbosity_debug );
        if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] < t_initialEnergy )
            index_lower = ( index_lower + index_upper ) / 2;
        else if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] > t_initialEnergy )
            index_upper = ( index_lower + index_upper ) / 2;
    }

    return { index_lower, index_upper };
}

double DetectorResponsePredictor::eval_hists_emission_values( const map< int, TH2D* >* t_hists_emission,
                                                              const double             t_initialEnergy ,
                                                              const double             t_trackLength   ,
                                                              const double             t_photonAngle    ) const {
    Log_debug( "    Evaluating emission histograms.", m_verbosity_debug );
    pair< int, int > closestHists{ get_closestEmissionHists( t_hists_emission, t_initialEnergy ) };

    // Use linear inerpolation between upper and lower histograms
    // to find expected emission value.
    int initialEnergy_lower{ m_hists_emission_initialEnergies[ closestHists.first ] };
    int initialEnergy_upper{ m_hists_emission_initialEnergies[ closestHists.second ] };
    double emission_lower{ t_hists_emission->at( initialEnergy_lower )->Interpolate( t_trackLength, t_photonAngle ) };
    double emission_upper{ t_hists_emission->at( initialEnergy_upper )->Interpolate( t_trackLength, t_photonAngle ) };
    double slope{ ( emission_upper - emission_upper ) / ( initialEnergy_lower - initialEnergy_upper ) };
    double value{ slope * ( t_initialEnergy - initialEnergy_lower ) + emission_lower }; // point slope
    Log_debug( "    Interpolated value=" + to_string( value ) + ".", m_verbosity_debug );
    return value;
}

double DetectorResponsePredictor::eval_hists_emission_indicies( const map< int, TH2D* >* t_hists_emission  ,
                                                                const double             t_initialEnergy   ,
                                                                const unsigned int       t_trackLengthIndex, 
                                                                const unsigned int       t_photonAngleIndex ) const {
    Log_debug( "    Evaluating emission histograms.", m_verbosity_debug );
    pair< int, int > closestHists{ get_closestEmissionHists( t_hists_emission, t_initialEnergy ) };

    // Use linear inerpolation between upper and lower histograms
    // to find expected emission value.
    int initialEnergy_lower{ m_hists_emission_initialEnergies[ closestHists.first ] };
    int initialEnergy_upper{ m_hists_emission_initialEnergies[ closestHists.second ] };
    double emission_lower{ t_hists_emission->at( initialEnergy_lower )->GetBinContent( t_trackLengthIndex, t_photonAngleIndex ) };
    double emission_upper{ t_hists_emission->at( initialEnergy_upper )->GetBinContent( t_trackLengthIndex, t_photonAngleIndex ) };
    double slope{ ( emission_upper - emission_upper ) / ( initialEnergy_lower - initialEnergy_upper ) };
    double value{ slope * ( t_initialEnergy - initialEnergy_lower ) + emission_lower }; // point slope
    Log_debug( "    Interpolated value=" + to_string( value ) + ".", m_verbosity_debug );
    return value;
}
 
inline double DetectorResponsePredictor::eval_hists_emission_tankWater( const double t_initialEnergy,
                                                                        const double t_trackLength  , 
                                                                        const double t_photonAngle   ) const {
    Log_debug( "Evaluating tank water emission histograms.", m_verbosity_debug );
    return eval_hists_emission_values( m_hists_emission_tankWater, t_initialEnergy, t_trackLength, t_photonAngle );
}

inline double DetectorResponsePredictor::eval_hists_emission_MRDsci( const double t_initialEnergy,
                                                                     const double t_trackLength  , 
                                                                     const double t_photonAngle   ) const {
    Log_debug( "Evaluating MRD scintilator emission histograms.", m_verbosity_debug );
    return eval_hists_emission_values( m_hists_emission_MRDsci, t_initialEnergy, t_trackLength, t_photonAngle );
}

double DetectorResponsePredictor::eval_hist( const TH1D  * t_hist,
                                             const double  t_x   ) const {
    Log_debug( "    Evaluating histogram", m_verbosity_debug );
    return t_hist->Interpolate( t_x );
}

inline double DetectorResponsePredictor::eval_hist_transmission_tankWater( const double t_photonEnergy ) const {
    Log_debug( "Evaluating tank water transmission histogram.", m_verbosity_debug );
    return eval_hist( m_hist_transmission_tankWater, t_photonEnergy );
}

inline double DetectorResponsePredictor::eval_hist_transmission_MRDsci( const double t_photonEnergy ) const {
    Log_debug( "Evaluating MRD scintilator transmission histogram.", m_verbosity_debug );
    return eval_hist( m_hist_transmission_tankWater, t_photonEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_tankWater( const double t_primaryEnergy ) const {
    Log_debug( "Evaluating MRD scintilator dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_tankWater, t_primaryEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_tankSteel( const double t_primaryEnergy ) const {
    Log_debug( "Evaluating tank steel dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_tankSteel, t_primaryEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_MRDsci( const double t_primaryEnergy ) const {
    Log_debug( "Evaluating MRD scinitilator dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_MRDsci, t_primaryEnergy );
}

inline double DetectorResponsePredictor::eval_hist_dEdX_MRDiron( const double t_primaryEnergy ) const {
    Log_debug( "Evaluating MRD ironr dEdX histogram.", m_verbosity_debug );
    return eval_hist( m_hist_dEdX_MRDiron, t_primaryEnergy );
}
    
inline void DetectorResponsePredictor::Log_debug( const string& t_message, const int t_verbosity ) const {
    if( t_verbosity >= m_verbosity )
        cout << "DetectorResponsePredictor || " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message;
}

inline void DetectorResponsePredictor::Log_debug( const string&& t_message, const int t_verbosity ) const {
    if( t_verbosity >= m_verbosity )
        cout << "DetectorResponsePredictor || " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message;
}

inline double DetectorResponsePredictor::get_angle( const TVector3& t_vector_1, const TVector3& t_vector_2 ) const {
    return acos( t_vector_1.Dot( t_vector_2 ) / ( t_vector_1.Mag() * t_vector_1.Mag() ) ); // =[0,pi]
}

inline double DetectorResponsePredictor::get_distance( const TVector3& t_point_1, const TVector3& t_point_2 ) const {
    return ( t_point_2 - t_point_1 ).Mag();
}

inline void DetectorResponsePredictor::normalize( TVector3& t_vector ) const {
    t_vector = t_vector.Unit();
}

int DetectorResponsePredictor::get_bin_index( const double t_value, const double t_min  , 
                                              const double t_max  , const int    t_nBins ) const {
    if( t_value < t_min || t_value > t_max )
        return -1;
    return int( ( t_value - t_min ) / ( ( t_max - t_min ) / t_nBins ) );
}

int DetectorResponsePredictor::get_bin_index( const double t_value, const TAxis* t_axis ) const {
    double min  { t_axis->GetXmin () };
    double max  { t_axis->GetXmax () };
    int    nBins{ t_axis->GetNbins() };

    if( t_value < min || t_value > max )
        return -1;
    return int( ( t_value - min ) / ( ( max - min ) / nBins ) );
}

double DetectorResponsePredictor::get_bin_value( const unsigned int t_index, const double t_min  , 
                                                 const double       t_max  , const int    t_nBins ) const {
    if( t_index >= t_nBins )
        return -1;
    return ( t_max - t_min ) / t_nBins * ( t_index + 0.5 ) + t_min;
}

double DetectorResponsePredictor::get_bin_value( const unsigned int t_index, const TAxis* t_axis ) const {
    double min  { t_axis->GetXmin () };
    double max  { t_axis->GetXmax () };
    int    nBins{ t_axis->GetNbins() };

    if( t_index >= nBins )
        return -1;
    return ( max - min ) / nBins * ( t_index + 0.5 ) + min;
}

index_3 DetectorResponsePredictor::get_bin_index_detector_center( const TVector3& t_particle_position_init, const TVector3& t_particle_direction, 
                                                                  const TVector3& t_detector_position     , const TH2D    * t_hist_emission     , 
                                                                  const double    t_particle_trackLength                                         ) const {
    index_3 returnVariable;

    // x-axis (s)
    returnVariable.x = get_bin_index( t_particle_trackLength, t_hist_emission->GetXaxis() );

    // y-axis (theta)
    TVector3 particle_position_current = t_particle_position_init + t_particle_direction.Unit() * t_particle_trackLength; // current particle position
    TVector3 particleToDetector = t_detector_position - particle_position_current; // vector pointing from current particle position to detector position
    returnVariable.y = get_bin_index( get_angle( particleToDetector, t_particle_direction ), t_hist_emission->GetYaxis() );

    // z-axis (phi)
    returnVariable.z = get_bin_index( get_angle( particleToDetector, m_vector_y ), 0, 2 * M_PI, m_binWidth_phi_MRDsci );

    return returnVariable;
}
    
double DetectorResponsePredictor::get_expected_height( Particle* t_particle, Detector* t_detector ) const {
    TVector3 particle_position_init( t_particle->GetStartVertex      ().X(),
                                     t_particle->GetStartVertex      ().Y(),
                                     t_particle->GetStartVertex      ().Z() );
    TVector3 particle_direction    ( t_particle->GetStartDirection   ().X(),
                                     t_particle->GetStartDirection   ().Y(),
                                     t_particle->GetStartDirection   ().Z() );
    TVector3 detector_position     ( t_detector->GetDetectorPosition ().X(),
                                     t_detector->GetDetectorPosition ().Y(),
                                     t_detector->GetDetectorPosition ().Z() );
    TVector3 detector_direction    ( t_detector->GetDetectorDirection().X(),
                                     t_detector->GetDetectorDirection().Y(),
                                     t_detector->GetDetectorDirection().Z() );
    normalize( detector_direction );
    normalize( particle_direction );
    const double particle_energy_init{ t_particle->GetStartEnergy() * 1000 }; // Convert from GeV to MeV

    bool   ( DetectorResponsePredictor::*get_isDetector    )( const TVector3&, const TVector3&, const TVector3&, const TVector3&, const TH2D*, const index_3 ) const;
    double ( DetectorResponsePredictor::*get_transmittance )( const double, const double ) const;
    double ( DetectorResponsePredictor::*get_acceptance    )( const double, const int ) const;
    double phiBinWidth;
    map< int, TH2D* >* hists_emission;
    map< int, TH2D* >* hists_emission_energies;
    if( t_detector->GetDetectorElement() == "PMT" ) {
        hists_emission = m_hists_emission_tankWater;
        hists_emission_energies = m_hists_emission_tankWater_energies;
        phiBinWidth = m_binWidth_phi_tankWater;
        get_isDetector = &DetectorResponsePredictor::get_isDetector_tankPMT;
        get_transmittance = &DetectorResponsePredictor::get_transmittance_tankWater;
        get_acceptance = &DetectorResponsePredictor::get_acceptance_tankPMT;
    } else if( t_detector->GetDetectorElement() == "LAPPD" ) {
        hists_emission = m_hists_emission_tankWater;
        phiBinWidth = m_binWidth_phi_tankWater;
        hists_emission_energies = m_hists_emission_tankWater_energies;
        get_isDetector = &DetectorResponsePredictor::get_isDetector_LAPPD;
        get_transmittance = &DetectorResponsePredictor::get_transmittance_tankWater;
        get_acceptance = &DetectorResponsePredictor::get_acceptance_LAPPD;
    } else if( t_detector->GetDetectorElement() == "MRD" ) {
        hists_emission = m_hists_emission_MRDsci;
        phiBinWidth = m_binWidth_phi_MRDsci;
        hists_emission_energies = m_hists_emission_MRDsci_energies;
        get_isDetector = &DetectorResponsePredictor::get_isDetector_MRD;
        get_transmittance = &DetectorResponsePredictor::get_transmittance_MRDsci;
        get_acceptance = &DetectorResponsePredictor::get_acceptance_MRD;
    } else {
        Log_debug( "Detector::DetectorElement is not \"PMT\", \"LAPPD\", or \"MRD\", it is \"" + t_detector->GetDetectorElement() + "\". Returning 0.", m_verbosity_debug );
        return 0;
    }
    TH2D* firstHist = hists_emission->at( 0 );

    int nBins[ 3 ]{ firstHist->GetNbinsX(), firstHist->GetNbinsY(), firstHist->GetNbinsY() };
    index_3 index_searching;
    index_3 index_detectorCenter = get_bin_index_detector_center( particle_position_init, particle_direction, detector_position, firstHist, 0 );
    index_3 index_current;
    queue < index_3 > indicies_searching( { index_detectorCenter } );
    bool checked[ nBins[ 0 ] ][ nBins[ 1 ] ][ nBins[ 2 ] ]; // initialize to false
    checked[ index_detectorCenter.x ][ index_detectorCenter.y ][ index_detectorCenter.z ] = true;
    double percentPhiBin = 1 / nBins[ 2 ];
    double height;
    TVector3 particleToDetector;
    
    while( !indicies_searching.empty() ) {
        index_searching = indicies_searching.front();
        if( ( this->*get_isDetector )( particle_position_init, particle_direction, detector_position, detector_direction, firstHist, index_searching ) ) {
            particleToDetector = detector_position - particle_position_init + particle_direction * get_bin_value( index_searching.x, firstHist->GetXaxis() );
            height += eval_hists_emission_indicies( hists_emission, particle_energy_init, index_searching.x, index_searching.y ) 
                    * ( this->*get_transmittance )( particleToDetector.Mag(), eval_hists_emission_indicies( hists_emission_energies, particle_energy_init, index_searching.x, index_searching.y ) )
                    * ( this->*get_acceptance )( get_angle( particleToDetector, detector_direction ), t_detector->GetDetectorID() );

            for( int dx{ -1 }; dx <= 1; dx++ )
                for( int dy{ -1 }; dy <= 1; dy++ )
                    for( int dz{ -1 }; dz <= 1; dz++ ) {
                        index_current = { index_searching.x + dx, index_searching.y + dy, index_searching.z + dz };
                        if( index_current.x >= 0 && index_current.x < nBins[ 0 ] &&
                            index_current.y >= 0 && index_current.y < nBins[ 1 ] &&
                            index_current.z >= 0 && index_current.z < nBins[ 2 ] &&
                            !checked[ index_current.x ][ index_current.y ][ index_current.z ] )
                            indicies_searching.push( index_current );
                    }
        }
        indicies_searching.pop();
    }

    return height;
}
    
double DetectorResponsePredictor::get_expected_time( Particle* t_particle, Detector* t_detector ) const
{
    return 0;
}
    
bool DetectorResponsePredictor::get_isDetector_tankPMT( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
                                                        const TVector3& t_detector_position     , const TVector3& t_detector_direction,
                                                        const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const {
    return true;
}

bool DetectorResponsePredictor::get_isDetector_LAPPD( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
                                                      const TVector3& t_detector_position     , const TVector3& t_detector_direction,
                                                      const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const {
    return true;
}

bool DetectorResponsePredictor::get_isDetector_MRD( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
                                                    const TVector3& t_detector_position     , const TVector3& t_detector_direction,
                                                    const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const {
    return true;
}
    
double DetectorResponsePredictor::get_acceptance_tankPMT( const double t_incidenceAngle, const int t_detectorID ) const {
    return 1;
}

double DetectorResponsePredictor::get_acceptance_LAPPD( const double t_incidenceAngle, const int t_detectorID ) const {
    return 1;
}

double DetectorResponsePredictor::get_acceptance_MRD( const double t_incidenceAngle, const int t_detectorID ) const {
    return 1;
}

double DetectorResponsePredictor::get_transmittance_tankWater( const double t_distance, const double t_photonEnergy ) const {
    return pow( eval_hist_transmission_tankWater( t_photonEnergy ), t_distance );
}

double DetectorResponsePredictor::get_transmittance_MRDsci( const double t_distance, const double t_photonEnergy ) const {
    return pow( eval_hist_transmission_MRDsci( t_photonEnergy ), t_distance );
}