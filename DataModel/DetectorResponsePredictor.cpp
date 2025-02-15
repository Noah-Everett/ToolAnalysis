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
//*//   transmission [1/m] (%/m)                                                             //*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//

// #ifndef DETECTORRESPONSEPREDICTOR_CPP
// #define DETECTORRESPONSEPREDICTOR_CPP

#include "DetectorResponsePredictor.h"
  
void DetectorResponsePredictor::reset_members()
{
    m_verbosity = m_verbosity_warning         ;
    m_hists_emission_initialEnergies.clear()  ;
}

bool DetectorResponsePredictor::check_axis( const TAxis* t_axis ) const {
    LogD( "Checking axis.", m_verbosity_debug );

    if( ! t_axis ) {
        LogD( "Axis is a nullptr.", m_verbosity_error );
        return false;
    }

    if( t_axis->GetXmin() >= t_axis->GetXmax() ) {
        LogD( "Axis min is greater than or equal to axis max.", m_verbosity_error );
        return false;
    }
    if( t_axis->GetNbins() <= 0 ) {
        LogD( "Axis has less than or equal to 0 bins.", m_verbosity_error );
        return false;
    }

    return true;
}

bool DetectorResponsePredictor::check_axes( const vector< TAxis* >& t_axes, double t_min, double t_max, int t_nBins, double t_epsilon ) const {
    LogD( "Checking axes.", m_verbosity_debug );

    for( TAxis* axis : t_axes ) {
        if( ! check_axis( axis ) ) {
            LogD( "One of the axes is invalid.", m_verbosity_error );
            return false;
        }
    }

    if( t_min == 0 && t_max == 0 && t_nBins == 0 ) {
        LogD( "Setting `t_min`, `t_max`, and `t_nBins` to the values of the first axis.", m_verbosity_debug );
        t_min   = t_axes[ 0 ]->GetXmin();
        t_max   = t_axes[ 0 ]->GetXmax();
        t_nBins = t_axes[ 0 ]->GetNbins();
    }

    for( TAxis* axis : t_axes ) {
        if( abs( axis->GetXmin() - t_min ) > t_epsilon ||
            abs( axis->GetXmax() - t_max ) > t_epsilon ||
            axis->GetNbins() != t_nBins ) {
            LogD( "All axes do not have the same min, max, and bin counts.", m_verbosity_error );
            LogD( "With threashold `epsilon` = " + to_string( t_epsilon ) + ", one of the following is true:", m_verbosity_debug );
            LogD( "(axis->GetXmin() = "  + to_string( axis->GetXmin () ) + ") != (t_min = "   + to_string( t_min   ) + ")", m_verbosity_debug );
            LogD( "(axis->GetXmax() = "  + to_string( axis->GetXmax () ) + ") != (t_max = "   + to_string( t_max   ) + ")", m_verbosity_debug );
            LogD( "(axis->GetNbins() = " + to_string( axis->GetNbins() ) + ") != (t_nBins = " + to_string( t_nBins ) + ")", m_verbosity_debug );
            return false;
        }
    }

    return true;
}

template< typename type_ID, typename type_hist >
bool DetectorResponsePredictor::load_hists(       shared_ptr< THistMap< type_ID, type_hist > >& t_hists      ,
                                            const vector              < string               >& t_hists_paths,
                                            const vector              < string               >& t_hists_names,
                                            const vector              < type_ID              >& t_hists_IDs   ) {
    LogD( "Loading histograms.", m_verbosity_debug );

    // Check IDs are strictly increasing
    for( int i{ 0 }; i + 1 < t_hists_IDs.size(); i++ ) {
        if( t_hists_IDs[ i ] >= t_hists_IDs[ i + 1 ] ) {
            LogD( "`t_hists_IDs` does not have strictly increasing entries.", m_verbosity_error );
            string IDs = "t_hists_IDs = [ ";
            for( type_ID ID : t_hists_IDs )
                IDs += to_string( ID ) + ", ";
            IDs += "]";
            LogD( IDs, m_verbosity_debug );
            return false;
        }
    }

    // Check vectors size
    if( t_hists_paths.empty() || t_hists_names.empty() || t_hists_IDs.empty() ) {
        LogD( "Arguments `t_hists_paths`, `t_hists_names`, and `t_hists_IDs` should not be empty.", m_verbosity_error );
        goto coutSizes;
    } else if( ( t_hists_paths.size() != t_hists_names.size() )
            || ( t_hists_paths.size() != t_hists_IDs  .size() )
            || ( t_hists_names.size() != t_hists_IDs  .size() ) ) {
        LogD( "Arguments `t_hists_paths`, `t_hists_names`, and `t_hists_IDs` should be the same size.", m_verbosity_error );
        goto coutSizes;
    }
    if( false ) {
        coutSizes:
        LogD( "t_hists_paths.size() = " + to_string( t_hists_paths.size() ), m_verbosity_debug );
        LogD( "t_hists_names.size() = " + to_string( t_hists_names.size() ), m_verbosity_debug );
        LogD( "t_hists_IDs  .size() = " + to_string( t_hists_IDs  .size() ), m_verbosity_debug );
        return false;
    }

    // Load hist map
    THistReader< type_ID, type_hist >* histReader{ new THistReader< type_ID, type_hist >( t_hists_paths, t_hists_IDs, t_hists_names, m_particle_name, m_verbosity_THistReader ) };
    if( ! histReader ) {
        LogD( "Failed to load histograms. THistReader failed to initialize.", m_verbosity_error );
        return false;
    }

    // Check that all histograms were loaded
    auto histsMap = histReader->get_histsMap();
    delete histReader;
    if( ! histsMap ) {
        LogD( "`histReader->get_histsMap()` returned a nullptr.", m_verbosity_error );
        return false;
    } else if( histsMap->size() != t_hists_paths.size() ) {
        LogD( "All histograms in `t_hists` were not loaded.", m_verbosity_error );
        return false;
    } else if( ! histsMap->at( 0 ) ) {
        LogD( "Histogram with ID 0 was not loaded.", m_verbosity_error );
        return false;
    }

    // Check that all histograms have the same axes
    vector< TAxis* > axes_x;
    vector< TAxis* > axes_y;
    for( pair< type_ID, type_hist* > hist : *histsMap ) { // replaced structured binding to avoid warning
        type_hist* const& h = hist.second;
        axes_x.push_back( h->GetXaxis() );
        axes_y.push_back( h->GetYaxis() );
    }

    t_hists = histsMap;
    return true;
}

template< typename type_hist >
bool DetectorResponsePredictor::make_averageTH1( shared_ptr< THistMap< Energy, type_hist > > t_hists, shared_ptr< type_hist >& t_hist, TString t_name, TString t_title ) {
    LogD( "Making average histogram.", m_verbosity_debug );

    // Check that all histograms have the same axes
    vector< TAxis* > axes;
    for( pair< Energy, type_hist* > hist : *t_hists ) { // replaced structured binding to avoid warning
        type_hist* const& h = hist.second;
        axes.push_back( h->GetXaxis() );
    }
    if( ! check_axes( axes ) ) {
        LogD( "All histograms do not have the same axes.", m_verbosity_error );
        return false;
    }

    // Make average histogram
    if( t_name.EqualTo( "" ) )
        t_name = t_hists->at( 0 )->GetName();
    if( t_title.EqualTo( "" ) )
        t_title = t_hists->at( 0 )->GetTitle();
    vector< Energy > keys;
    for( pair< Energy, type_hist* > hist : *t_hists ) // replaced structured binding to avoid warning
        keys.push_back( hist.first );
    int nBins = keys.size();
    double min = keys[ 0 ];
    double max = keys[ 0 ];
    for( int key : keys ) {
        if( key < min )
            min = key;
        if( key > max )
            max = key;
    }

    t_hist = make_shared< type_hist >( type_hist( t_name, t_title, nBins, min, max ) );
    for( int key : keys ) {
        type_hist* hist = t_hists->at( key );
        double avg = hist->GetMean();
        int    bin = t_hist->FindBin( key );
        t_hist->SetBinContent( bin, avg );
        t_hist->SetBinError  ( bin, 0   ); // Can change later if needed
    }

    return true;
}

bool DetectorResponsePredictor::load_hists_transmission_tankWater( const vector< string >& t_hists_paths   ,
                                                                   const vector< string >& t_hists_names   ,
                                                                   const vector< Energy >& t_hists_energies ) {
    LogD( "Loading tank water transmission histograms.", m_verbosity_debug );

    if( ! load_hists( m_hists_transmission_tankWater, t_hists_paths, t_hists_names, t_hists_energies ) ) {
        LogD( "Failed to load tank water transmission histograms.", m_verbosity_error );
        return false;
    }

    return make_averageTH1( m_hists_transmission_tankWater, m_hist_transmission_tankWater );
}

bool DetectorResponsePredictor::load_hists_transmission_MRDsci( const vector< string >& t_hists_paths   ,
                                                                const vector< string >& t_hists_names   ,
                                                                const vector< Energy >& t_hists_energies ) {
    LogD( "Loading MRD scintilator transmission histograms.", m_verbosity_debug );

    if( ! load_hists( m_hists_transmission_MRDsci, t_hists_paths, t_hists_names, t_hists_energies ) ) {
        LogD( "Failed to load MRD scintilator transmission histograms.", m_verbosity_error );
        return false;
    }

    return make_averageTH1( m_hists_transmission_MRDsci, m_hist_transmission_MRDsci );
}

bool DetectorResponsePredictor::load_hists_emission(       shared_ptr< THistMap< Energy, TH2D > >& t_hists_energies       ,
                                                           shared_ptr< THistMap< Energy, TH2D > >& t_hists_counts         ,
                                                     const vector    < string                   >& t_hists_energies_paths , 
                                                     const vector    < string                   >& t_hists_counts_paths   ,
                                                     const vector    < string                   >& t_hists_energies_names ,
                                                     const vector    < string                   >& t_hists_counts_names   ,
                                                     const vector    <           Energy         >& t_hists_initialEnergies,
                                                           double                                & t_binWidth_s           ,
                                                           double                                & t_binWidth_theta       ,
                                                           double                                & t_binWidth_phi         ,
                                                     const string                                & t_hists_tag             ) {
    LogD( "Loading emission histograms.", m_verbosity_debug );

    if( !m_hists_emission_initialEnergies.empty() && m_hists_emission_initialEnergies != t_hists_initialEnergies ) {
        LogD( "Argument `t_hists_initialEnergies` for `load_hists_emission_tankWater()`" 
                   " and `load_hists_emission_MRDsci()` should match.", m_verbosity_error );
        return false;
    }

    if( m_hists_emission_initialEnergies.empty() )
        m_hists_emission_initialEnergies = t_hists_initialEnergies;

    // Load histograms
    if( ! load_hists( t_hists_energies, t_hists_energies_paths, t_hists_energies_names, t_hists_IDs ) ) {
        LogD( "Failed to load emission energy histograms.", m_verbosity_error );
        return false;
    }
    if( ! load_hists( t_hists_counts  , t_hists_counts_paths  , t_hists_counts_names  , t_hists_IDs ) ) {
        LogD( "Failed to load emission count histograms.", m_verbosity_error );
        return false;
    }

    // Set initial energies
    m_hists_emission_initialEnergies = t_hists_IDs;

    // Set bin widths
    t_binWidth_s     = t_hists_energies->begin()->second->GetXaxis()->GetBinWidth( 0 );
    t_binWidth_theta = t_hists_energies->begin()->second->GetYaxis()->GetBinWidth( 0 );
    t_binWidth_phi   = t_hists_energies->begin()->second->GetYaxis()->GetBinWidth( 0 );

    return true;
}

bool DetectorResponsePredictor::load_hists_emission_tankWater( const vector< string >& t_hists_energies_paths,
                                                               const vector< string >& t_hists_counts_paths  ,
                                                               const vector< string >& t_hists_energies_names,
                                                               const vector< string >& t_hists_counts_names  ,
                                                               const vector< Energy >& t_hists_IDs            ) {
    LogD( "Loading tank water emission energy and count histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_tankWater_energies, m_hists_emission_tankWater_counts,
                                t_hists_energies_paths             , t_hists_counts_paths             ,
                                t_hists_energies_names             , t_hists_counts_names             ,
                                t_hists_IDs                        , m_binWidth_s_tankWater           , 
                                m_binWidth_theta_tankWater         , m_binWidth_phi_tankWater         ,
                                m_particle_name + "_tankWater"                                         );
}

bool DetectorResponsePredictor::load_hists_emission_MRDsci( const vector< string >& t_hists_energies_paths,
                                                            const vector< string >& t_hists_counts_paths  ,
                                                            const vector< string >& t_hists_energies_names,
                                                            const vector< string >& t_hists_counts_names  ,
                                                            const vector< Energy >& t_hists_IDs            ) {
    LogD( "Loading MRD scintilator emission energy and count histograms.", m_verbosity_debug );
    return load_hists_emission( m_hists_emission_MRDsci_energies, m_hists_emission_MRDsci_counts,
                                t_hists_energies_paths          , t_hists_counts_paths          ,
                                t_hists_energies_names          , t_hists_counts_names          ,
                                t_hists_IDs                     , m_binWidth_s_MRDsci           , 
                                m_binWidth_theta_MRDsci         , m_binWidth_phi_MRDsci         ,
                                m_particle_name + "_MRDsci"                                      );
}

template< typename type_hist >
bool DetectorResponsePredictor::load_hist(       shared_ptr< type_hist >& t_hist     ,
                                           const string                 & t_hist_path,
                                           const string                 & t_hist_name,
                                           const string                 & t_hist_tag  ) {
    LogD( "Loading histogram with path " + t_hist_path + " and name " + t_hist_name + ".", m_verbosity_debug );

    // Load histogram
    shared_ptr< THistMap< bool, type_hist > > temp;
    if( ! load_hists( temp, { t_hist_path }, { t_hist_name }, { 0 } ) ) {
        LogD( "Failed to load histogram.", m_verbosity_error );
        return false;
    }

    t_hist = shared_ptr< type_hist >( copy_TH( temp->at( 0 ), m_verbosity_THistReader ) );

    return true;
}

bool DetectorResponsePredictor::load_hist_dEdX_tankWater( const string& t_hist_path,
                                                          const string& t_hist_name ) {
    LogD( "Loading tank water dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_tankWater, t_hist_path, t_hist_name, m_particle_name + "_tankWater" );
}

bool DetectorResponsePredictor::load_hist_dEdX_tankSteel( const string& t_hist_path,
                                                          const string& t_hist_name ) {
    LogD( "Loading tank steel dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_tankSteel, t_hist_path, t_hist_name, m_particle_name + "_tankSteel" );
}

bool DetectorResponsePredictor::load_hist_dEdX_MRDsci( const string& t_hist_path,
                                                       const string& t_hist_name ) {
    LogD( "Loading MRD scintilator dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_MRDsci, t_hist_path, t_hist_name, m_particle_name + "_MRDsci" );
}

bool DetectorResponsePredictor::load_hist_dEdX_MRDiron( const string& t_hist_path,
                                                        const string& t_hist_name ) {
    LogD( "Loading MRD iron dEdX histogram.", m_verbosity_debug );
    return load_hist< TH1D >( m_hist_dEdX_MRDiron, t_hist_path, t_hist_name, m_particle_name + "_MRDiron" );
}

pair< int, int > DetectorResponsePredictor::get_closestEmissionHists( const shared_ptr< THistMap< Energy, TH2D > > t_hists_emission,
                                                                      const Energy                                 t_initialEnergy  ) const {
    // Bisect histogram energies to find the histograms 
    // which have energies just lower and just higher 
    // than t_initialEnergy.
    int index_lower{ 0 }, index_upper{ int(m_hists_emission_initialEnergies.size()) - 1 }; // typecast to int to avoid warning
    LogD( "Finding histograms with initial primary energies just below and above `t_initialEnergy` with bisection.", m_verbosity_debug );
    while( index_upper - index_lower != 1 ) {
        LogD( "[|]-Lower: index="  + to_string( index_lower ) + " && initEnergy=" + m_hists_emission_initialEnergies[ index_lower ] + "\n"
              " \\--Upper: index=" + to_string( index_upper ) + " && initEnergy=" + m_hists_emission_initialEnergies[ index_upper ], m_verbosity_debug );
        // LogD( m_temp_string + to_string( index_lower ) + " && initEnergy=", m_verbosity_debug );
        if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] < t_initialEnergy )
            index_lower = ( index_lower + index_upper ) / 2;
        else if( m_hists_emission_initialEnergies[ ( index_lower + index_upper ) / 2 ] > t_initialEnergy )
            index_upper = ( index_lower + index_upper ) / 2;
    }

    return { index_lower, index_upper };
}

double DetectorResponsePredictor::eval_hists_emission_values( const shared_ptr< THistMap< Energy, TH2D > > t_hists_emission,
                                                              const double                                 t_initialEnergy ,
                                                              const double                                 t_trackLength   ,
                                                              const double                                 t_photonAngle    ) const {
    LogD( "Evaluating emission histograms.", m_verbosity_debug );
    pair< int, int > closestHists{ get_closestEmissionHists( t_hists_emission, t_initialEnergy ) };

    // Use linear inerpolation between upper and lower histograms
    // to find expected emission value.
    Energy initialEnergy_lower{ m_hists_emission_initialEnergies[ closestHists.first ] };
    Energy initialEnergy_upper{ m_hists_emission_initialEnergies[ closestHists.second ] };
    double emission_lower{ t_hists_emission->at( initialEnergy_lower )->Interpolate( t_trackLength, t_photonAngle ) };
    double emission_upper{ t_hists_emission->at( initialEnergy_upper )->Interpolate( t_trackLength, t_photonAngle ) };
    double slope{ ( emission_upper - emission_upper ) / ( initialEnergy_lower - initialEnergy_upper ) };
    double value{ slope * ( t_initialEnergy - initialEnergy_lower ) + emission_lower }; // point slope
    LogD( "    Interpolated value=" + to_string( value ) + ".", m_verbosity_debug );
    return value;
}

double DetectorResponsePredictor::eval_hists_emission_indicies( const shared_ptr< THistMap< Energy, TH2D > > t_hists_emission,
                                                                const double                                 t_initialEnergy   ,
                                                                const unsigned int                           t_trackLengthIndex, 
                                                                const unsigned int                           t_photonAngleIndex ) const {
    LogD( "Evaluating emission histograms.", m_verbosity_debug );
    pair< int, int > closestHists{ get_closestEmissionHists( t_hists_emission, t_initialEnergy ) };

    // Use linear inerpolation between upper and lower histograms
    // to find expected emission value.
    Energy initialEnergy_lower{ m_hists_emission_initialEnergies[ closestHists.first ] };
    Energy initialEnergy_upper{ m_hists_emission_initialEnergies[ closestHists.second ] };
    double emission_lower{ t_hists_emission->at( initialEnergy_lower )->GetBinContent( t_trackLengthIndex, t_photonAngleIndex ) };
    double emission_upper{ t_hists_emission->at( initialEnergy_upper )->GetBinContent( t_trackLengthIndex, t_photonAngleIndex ) };
    double slope{ ( emission_upper - emission_upper ) / ( initialEnergy_lower - initialEnergy_upper ) };
    double value{ slope * ( t_initialEnergy - initialEnergy_lower ) + emission_lower }; // point slope
    LogD( "    Interpolated value=" + to_string( value ) + ".", m_verbosity_debug );
    return value;
}
 
double DetectorResponsePredictor::eval_hists_emission_tankWater_energies( const double t_initialEnergy,
                                                                          const double t_trackLength  , 
                                                                          const double t_photonAngle   ) const {
    LogD( "Evaluating tank water emission energy histograms.", m_verbosity_debug );
    return eval_hists_emission_values( m_hists_emission_tankWater_energies, t_initialEnergy, t_trackLength, t_photonAngle );
}

double DetectorResponsePredictor::eval_hists_emission_tankWater_counts( const double t_initialEnergy,
                                                                        const double t_trackLength  , 
                                                                        const double t_photonAngle   ) const {
    LogD( "Evaluating tank water emission count histograms.", m_verbosity_debug );
    return eval_hists_emission_values( m_hists_emission_tankWater_counts, t_initialEnergy, t_trackLength, t_photonAngle );
}

double DetectorResponsePredictor::eval_hists_emission_MRDsci_energies( const double t_initialEnergy,
                                                                       const double t_trackLength  , 
                                                                       const double t_photonAngle   ) const {
    LogD( "Evaluating MRD scintilator emission energy histograms.", m_verbosity_debug );
    return eval_hists_emission_values( m_hists_emission_MRDsci_energies, t_initialEnergy, t_trackLength, t_photonAngle );
}

double DetectorResponsePredictor::eval_hists_emission_MRDsci_counts( const double t_initialEnergy,
                                                                     const double t_trackLength  , 
                                                                     const double t_photonAngle   ) const {
    LogD( "Evaluating MRD scintilator emission count histograms.", m_verbosity_debug );
    return eval_hists_emission_values( m_hists_emission_MRDsci_counts, t_initialEnergy, t_trackLength, t_photonAngle );
}

double DetectorResponsePredictor::eval_hist_value( const shared_ptr< TH1D >& t_hist,
                                                   const double              t_x    ) const {
    LogD( "Evaluating histogram", m_verbosity_debug );
    return t_hist->Interpolate( t_x );
}

double DetectorResponsePredictor::eval_hist_index( const shared_ptr< TH1D >& t_hist,
                                                   const unsigned int        t_xIndex ) const {
    LogD( "Evaluating histogram", m_verbosity_debug );
    return t_hist->GetBinContent( t_xIndex );
}

double DetectorResponsePredictor::eval_hist_transmission_tankWater( const double t_photonEnergy ) const {
    LogD( "Evaluating tank water transmission histogram.", m_verbosity_debug );
    return eval_hist_value( m_hist_transmission_tankWater, t_photonEnergy );
}

double DetectorResponsePredictor::eval_hist_transmission_MRDsci( const double t_photonEnergy ) const {
    LogD( "Evaluating MRD scintilator transmission histogram.", m_verbosity_debug );
    return eval_hist_value( m_hist_transmission_MRDsci, t_photonEnergy );
}

double DetectorResponsePredictor::eval_hist_dEdX_tankWater( const double t_primaryEnergy ) const {
    LogD( "Evaluating MRD scintilator dEdX histogram.", m_verbosity_debug );
    return eval_hist_value( m_hist_dEdX_tankWater, t_primaryEnergy );
}

double DetectorResponsePredictor::eval_hist_dEdX_tankSteel( const double t_primaryEnergy ) const {
    LogD( "Evaluating tank steel dEdX histogram.", m_verbosity_debug );
    return eval_hist_value( m_hist_dEdX_tankSteel, t_primaryEnergy );
}

double DetectorResponsePredictor::eval_hist_dEdX_MRDsci( const double t_primaryEnergy ) const {
    LogD( "Evaluating MRD scinitilator dEdX histogram.", m_verbosity_debug );
    return eval_hist_value( m_hist_dEdX_MRDsci, t_primaryEnergy );
}

double DetectorResponsePredictor::eval_hist_dEdX_MRDiron( const double t_primaryEnergy ) const {
    LogD( "Evaluating MRD ironr dEdX histogram.", m_verbosity_debug );
    return eval_hist_value( m_hist_dEdX_MRDiron, t_primaryEnergy );
}
    
inline void DetectorResponsePredictor::Log_debug( const string& t_message, unsigned int t_verbosity,
                                                  const string& t_file   , const string& t_function, int t_line ) const {
    if( t_verbosity <= m_verbosity )
        cout << "DataModel_DetectorResponsePredictor  || " << m_verbosity_map.at( t_verbosity ) << " || "
             << t_file << "::" << t_function << " (" << t_line << "): " << t_message << endl;
}

inline void DetectorResponsePredictor::Log_debug( const string&& t_message, unsigned int t_verbosity,
                                                  const string&  t_file   , const string& t_function, int t_line ) const {
    if( t_verbosity <= m_verbosity )
        cout << "DataModel_DetectorResponsePredictor  || " << m_verbosity_map.at( t_verbosity ) << " || " 
             << t_file << "::" << t_function << " (" << t_line << "): " << t_message << endl;
}

double DetectorResponsePredictor::get_angle( const TVector3& t_vector_1, const TVector3& t_vector_2 ) const {
    return acos( t_vector_1.Dot( t_vector_2 ) / ( t_vector_1.Mag() * t_vector_1.Mag() ) ); // =[0,pi]
}

double DetectorResponsePredictor::get_distance( const TVector3& t_point_1, const TVector3& t_point_2 ) const {
    return ( t_point_2 - t_point_1 ).Mag();
}

void DetectorResponsePredictor::normalize( TVector3& t_vector ) const {
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
    
double DetectorResponsePredictor::get_expected_height( Particle* t_particle, Detector* t_detector, double t_time_start, double t_time_stop ) const {
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
    double ( DetectorResponsePredictor::*get_transmission )( const double, const double ) const;
    double ( DetectorResponsePredictor::*get_acceptance    )( const double, const int ) const;
    double phiBinWidth;
    shared_ptr< THistMap< Energy, TH2D > > hists_emission_counts;
    shared_ptr< THistMap< Energy, TH2D > > hists_emission_energies;
    if( t_detector->GetDetectorElement() == "PMT" ) {
        hists_emission_energies = m_hists_emission_tankWater_energies;
        hists_emission_counts   = m_hists_emission_tankWater_counts;
        phiBinWidth = m_binWidth_phi_tankWater;
        get_isDetector = &DetectorResponsePredictor::get_isDetector_tankPMT;
        get_transmission = &DetectorResponsePredictor::get_transmission_tankWater;
        get_acceptance = &DetectorResponsePredictor::get_acceptance_tankPMT;
    } else if( t_detector->GetDetectorElement() == "LAPPD" ) {
        hists_emission_energies = m_hists_emission_tankWater_energies;
        hists_emission_counts = m_hists_emission_tankWater_counts;
        phiBinWidth = m_binWidth_phi_tankWater;
        get_isDetector = &DetectorResponsePredictor::get_isDetector_LAPPD;
        get_transmission = &DetectorResponsePredictor::get_transmission_tankWater;
        get_acceptance = &DetectorResponsePredictor::get_acceptance_LAPPD;
    } else if( t_detector->GetDetectorElement() == "MRD" ) {
        hists_emission_energies = m_hists_emission_MRDsci_energies;
        hists_emission_counts = m_hists_emission_MRDsci_counts;
        phiBinWidth = m_binWidth_phi_MRDsci;
        hists_emission_energies = m_hists_emission_MRDsci_energies;
        get_isDetector = &DetectorResponsePredictor::get_isDetector_MRD;
        get_transmission = &DetectorResponsePredictor::get_transmission_MRDsci;
        get_acceptance = &DetectorResponsePredictor::get_acceptance_MRD;
    } else {
        LogD( "Detector::DetectorElement is not \"PMT\", \"LAPPD\", or \"MRD\", it is \"" + t_detector->GetDetectorElement() + "\". Returning 0.", m_verbosity_debug );
        return 0;
    }
    const TH2D  * referenceHist         { hists_emission_counts->at( 0 ) };
    const TAxis * referenceHist_xAxis   { referenceHist->GetXaxis () };
    unsigned int  refenceHist_nBins[ 3 ]{ static_cast<unsigned int>(referenceHist->GetNbinsX()), // typecast to unsigned int to avoid warning
                                          static_cast<unsigned int>(referenceHist->GetNbinsY()), 
                                          static_cast<unsigned int>(referenceHist->GetNbinsY()) };

    double x_value_min, x_value_max;
    if( t_time_start < 0 ) x_value_min = 0;
    else                   x_value_min = get_particlePosition_value( t_particle, t_time_start );
    if( t_time_stop  < 0 ) x_value_max = referenceHist->GetXaxis()->GetNbins() - 1;
    else                   x_value_max = get_particlePosition_value( t_particle, t_time_stop );
    int x_index_min{ get_bin_index( x_value_min, referenceHist_xAxis ) },
        x_index_max{ get_bin_index( x_value_min, referenceHist_xAxis ) };

    index_3 index_searching;
    index_3 index_detectorCenter{ get_bin_index_detector_center( particle_position_init, particle_direction, detector_position, referenceHist, x_value_min ) };
    index_3 index_current;
    queue < index_3 > indicies_searching( { index_detectorCenter } );
    bool checked[ refenceHist_nBins[ 0 ] ][ refenceHist_nBins[ 1 ] ][ refenceHist_nBins[ 2 ] ]; // initialize to false
    checked[ index_detectorCenter.x ][ index_detectorCenter.y ][ index_detectorCenter.z ] = true;
    double height;
    TVector3 particleToDetector;

    while( !indicies_searching.empty() ) {
        index_searching = indicies_searching.front();
        if( ( this->*get_isDetector )( particle_position_init, particle_direction, detector_position, detector_direction, referenceHist, index_searching ) ) {
            particleToDetector = detector_position - particle_position_init + particle_direction * get_bin_value( index_searching.x, referenceHist->GetXaxis() );
            height += eval_hists_emission_indicies( hists_emission_counts, particle_energy_init, index_searching.x, index_searching.y ) 
                    * ( this->*get_transmission )( particleToDetector.Mag(), eval_hists_emission_indicies( hists_emission_energies, particle_energy_init, index_searching.x, index_searching.y ) )
                    * ( this->*get_acceptance )( get_angle( particleToDetector, detector_direction ), t_detector->GetDetectorID() );

            for( int dx{ -1 }; dx <= 1; dx++ )
                for( int dy{ -1 }; dy <= 1; dy++ )
                    for( int dz{ -1 }; dz <= 1; dz++ ) {
                        index_current = { index_searching.x + dx, index_searching.y + dy, index_searching.z + dz };
                        if( index_current.x >= x_index_min && index_current.x <= x_index_max             &&
                            index_current.y >= 0           && index_current.y <  refenceHist_nBins[ 1 ]  &&
                            index_current.z >= 0           && index_current.z <  refenceHist_nBins[ 2 ]  &&
                            !checked[ index_current.x ][ index_current.y ][ index_current.z ] )
                            indicies_searching.push( index_current );
                    }
        }
        indicies_searching.pop();
    }

    return height;
}
    
double DetectorResponsePredictor::get_expected_time( Particle* t_particle, Detector* t_detector ) const {
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

double DetectorResponsePredictor::get_transmission_tankWater( const double t_distance, const double t_photonEnergy ) const {
    return pow( eval_hist_transmission_tankWater( t_photonEnergy ), t_distance );
}

double DetectorResponsePredictor::get_transmission_MRDsci( const double t_distance, const double t_photonEnergy ) const {
    return pow( eval_hist_transmission_MRDsci( t_photonEnergy ), t_distance );
}

double DetectorResponsePredictor::get_particle_mass( Particle* t_particle ) const {
    return m_map_particleMasses->at( t_particle->GetPdgCode() );
}

double DetectorResponsePredictor::get_particlePosition_value( Particle* t_particle, double t_time ) const {
    double mass{ get_particle_mass( t_particle ) };
    const TAxis* x_axis{ m_hist_dEdX_tankWater->GetXaxis() };
    const unsigned int x_index_max{ static_cast< unsigned int >(x_axis->GetNbins()) };
    const double x_bin_width{ x_axis->GetBinWidth( 0 ) };

    double time_cur{ 0 };
    double x_value_cur{ 0 };
    double speed_cur{ 0 };
    double energy_cur{ t_particle->GetStartEnergy() };
    for( unsigned int x_index_cur{ 0 }; x_index_cur < x_index_max && time_cur < t_time; x_index_cur++ ) {
        speed_cur = sqrt( pow( energy_cur, 2 ) - pow( mass, 2 ) * pow( m_c, 4 ) ) / ( m_c * mass );
        if( time_cur + x_bin_width / speed_cur >= t_time )
            return x_value_cur + ( t_time - time_cur ) * speed_cur;

        time_cur    += x_bin_width / speed_cur;
        energy_cur  += eval_hist_index( m_hist_dEdX_tankWater, x_index_cur );
        x_value_cur += x_bin_width;
    }

    return -1; // outside of hist
}

unsigned int DetectorResponsePredictor::get_particlePosition_index( Particle* t_particle, double t_time ) const {
    double mass{ get_particle_mass( t_particle ) };
    const TAxis* x_axis{ m_hist_dEdX_tankWater->GetXaxis() };
    const unsigned int x_index_max{ static_cast< unsigned int >(x_axis->GetNbins()) };
    const double x_bin_width{ x_axis->GetBinWidth( 0 ) };

    double time_cur{ 0 };
    double speed_cur{ 0 };
    double energy_cur{ t_particle->GetStartEnergy() };
    for( unsigned int x_index_cur{ 0 }; x_index_cur < x_index_max && time_cur < t_time; x_index_cur++ ) {
        speed_cur = sqrt( pow( energy_cur, 2 ) - pow( mass, 2 ) * pow( m_c, 4 ) ) / ( m_c * mass );
        if( time_cur + x_bin_width / speed_cur >= t_time )
            return x_index_cur;

        time_cur   += x_bin_width / speed_cur;
        energy_cur += eval_hist_index( m_hist_dEdX_tankWater, x_index_cur );
    }

    return -1; // outside of hist
}

// #endif