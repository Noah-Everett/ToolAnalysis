//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                               DetectorResponsePredictor.cpp                             //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*// Author:                                                                                 //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                                           //*//
//*//   Created Febuary 13, 2023                                                              //*//
//*//                                                                                         //*//
//*// About:                                                                                  //*//
//*//   This file contains classes for generalized detector response predicction. All         //*//
//*//   information (PDFs, histograms, etc.) needs to be set by the user.                     //*//
//*//                                                                                         //*//
//*// Classes:                                                                                //*//
//*//   DetectorResponsePredictor         --> Parent abstract class for the following classes //*//
//*//   DetectorResponsePredictor_TankPMT --> Class for predicting the responses of tank PMTs //*//
//*//   DetectorResponsePredictor_MRDPMT  --> Class for predicting the responses of MRD PMTs  //*//
//*//   DetectorResponsePredictor_LAPPD   --> Class for predicting the responses of LAPPDs    //*//
//*//                                                                                         //*//
//*// Notation:                                                                               //*//
//*//   t_ = function argument                                                                //*//
//*//   m_ = member variable                                                                  //*//
//*//   mainVariable_subCategory (use '_' like LaTeX)                                         //*//
//*//                                                                                         //*//
//*// Abreviations:                                                                           //*//
//*//   ptcl = particle                                                                       //*//
//*//   vtx  = vertex                                                                         //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

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

bool DetectorResponsePredictor::load_hists_emission( const map   < int, TH2D* >* t_hists_emission,
                                                     const vector< string     >& t_hists_paths   , 
                                                     const vector< int        >& t_hists_IDs     ,
                                                     const vector< string     >& t_hists_names   ) {
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
    m_histReader_TH2D = new THistReader< int, TH2D >( t_hists_paths, t_hists_IDs, t_hists_names );
    t_hists_emission = m_histReader_TH2D->get_histsMap_cp();
    if( m_histReader_TH2D ) {
        delete m_histReader_TH2D;
        return true;
    } else
        return false;
}

inline bool DetectorResponsePredictor::load_hists_emission_tankWater( const vector< string >& t_hists_paths, 
                                                                      const vector< int    >& t_hists_IDs  ,
                                                                      const vector< string >& t_hists_names ) {
    Log_debug( "Loading tank water emission histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_tankWater, t_hists_paths, t_hists_IDs, t_hists_names );
}

inline bool DetectorResponsePredictor::load_hists_emission_MRDsci( const vector< string >& t_hists_paths, 
                                                                   const vector< int    >& t_hists_IDs  ,
                                                                   const vector< string >& t_hists_names ) {
    Log_debug( "Loading MRD scintilator emission histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_MRDsci, t_hists_paths, t_hists_IDs, t_hists_names );
}

bool DetectorResponsePredictor::load_hist( const TH1D  *  m_hist     ,
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

double DetectorResponsePredictor::eval_hists_emission( const map< int, TH2D* >* t_hists_emission,
                                                       const double             t_initialEnergy ,
                                                       const double             t_trackLength   , 
                                                       const double             t_photonAngle   ) const {
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
 
inline double DetectorResponsePredictor::eval_hists_emission_tankWater( const double t_initialEnergy ,
                                                                        const double t_trackLength   , 
                                                                        const double t_photonAngle   ) const {
    Log_debug( "Evaluating tank water emission histograms.", m_verbosity_debug );
    return eval_hists_emission( m_hists_emission_tankWater, t_initialEnergy, t_trackLength, t_photonAngle );
}

inline double DetectorResponsePredictor::eval_hists_emission_MRDsci( const double t_initialEnergy ,
                                                                     const double t_trackLength   , 
                                                                     const double t_photonAngle   ) const {
    Log_debug( "Evaluating MRD scintilator emission histograms.", m_verbosity_debug );
    return eval_hists_emission( m_hists_emission_MRDsci, t_initialEnergy, t_trackLength, t_photonAngle );
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

inline double DetectorResponsePredictor::get_distance( const TVector3& t_point_1 , const TVector3& t_point_2  ) const {
    return ( t_point_2 - t_point_1 ).Mag();
}

pair< double, double >* DetectorResponsePredictor::get_emissionHistPoints( const TVector3& t_point             , 
                                                                           const TVector3& t_ptcl_position_init, 
                                                                           const TVector3& t_ptcl_direction    , 
                                                                           const double    t_ptcl_distance_max ,
                                                                           const int       t_nIterations       ) const {
    pair< double, double >* emissionHistPoints{ new pair< double, double >[ t_nIterations ] }; // < s, theta >

    double stepDistance = t_ptcl_distance_max / ( t_nIterations - 1 );
    for( int i{ 0 }; i < t_nIterations; i++ ) {
        emissionHistPoints[ i ].first = stepDistance * i;

        m_temp_TVector3 = t_ptcl_position_init + stepDistance * t_ptcl_direction.Unit(); // current ptcl position
        m_temp_TVector3 = t_point - m_temp_TVector3; // vector pointing from ptcl to PMT
        emissionHistPoints[ i ].second = get_angle( t_ptcl_direction, m_temp_TVector3 );
    }
    
    return emissionHistPoints;
}
    
inline TVector3 DetectorResponsePredictor::copy( const TVector3& t_TVector3 ) const {
    return TVector3( t_TVector3.X(), t_TVector3.Y(), t_TVector3.Z() );
}

inline TVector2 DetectorResponsePredictor::copy( const TVector2& t_TVector2 ) const {
    return TVector2( t_TVector2.X(), t_TVector2.Y() );
}

double DetectorResponsePredictor_TankPMT::get_expected_height( Particle* t_ptcl, int t_nDigit )
{
    // Unchanging variables
    const TVector3 PMT_position      ( m_vtxGeo->GetDigitX( t_nDigit )       , 
                                       m_vtxGeo->GetDigitY( t_nDigit )       , 
                                       m_vtxGeo->GetDigitZ( t_nDigit )       );
    const TVector3 ptcl_position_init( t_ptcl->GetStartVertex().X()          ,
                                       t_ptcl->GetStartVertex().Y()          ,
                                       t_ptcl->GetStartVertex().Z()          );
    const TVector2 ptcl_direction_2  ( t_ptcl->GetStartDirection().GetTheta(),
                                       t_ptcl->GetStartDirection().GetPhi()  );
    // theta --> clockwise looking down, 0 pointing downstream along beam
    // phi   --> pitch angle, relative to the xz-plane (beamline) 
    m_temp_TVector3.SetMagThetaPhi( 1, ptcl_direction_2.X(), ptcl_direction_2.Y() );
    TVector3 ptcl_direction_3( copy( m_temp_TVector3 ) ); // Use `copy()` as to not copy temp var by reference
    const double ptcl_energy_init{ t_ptcl->GetStartEnergy() / 1000 }; // [MeV] 
    
    // Changing function variables
    TVector3 ptcl_position_cur( copy( ptcl_position_init ) ); // Use `copy()` to to not copy var by reference
    double ptcl_s{ 0 };

    // Find theta_gamma of PMT center
    const int nPoints    { 4  };
    const int nIterations{ 10 };
    TVector3 PMTpoints[ nPoints ]; ////////////// INITALIZE ////////////////
    double distance_max[ nPoints ];////////////// INITALIZE ////////////////
    pair< double, double >* emissionHistPoints[ nIterations ];

    for( int i{ 0 }; i < nPoints; i++ )
        emissionHistPoints[ i ] = get_emissionHistPoints( PMTpoints[ i ], ptcl_position_init, ptcl_direction_3, distance_max[ i ], nIterations );

    for( int i{ 0 }; i < nPoints; i++ )
        delete [] emissionHistPoints[ i ];

    return 0;
}
    
double DetectorResponsePredictor_TankPMT::get_expected_time( Particle* t_particle, int t_nDigit )
{
    return 0;
}