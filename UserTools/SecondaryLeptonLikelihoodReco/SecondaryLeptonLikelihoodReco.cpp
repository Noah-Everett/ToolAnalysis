#include "boost/filesystem.hpp"

#include "SecondaryLeptonLikelihoodReco.h"

using boost::filesystem::exists;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;

bool SecondaryLeptonLikelihoodReco::Initialise( std::string configfile, DataModel &data )
{
    // Load config file
    if( configfile != "" ) m_variables.Initialise( configfile ); // loading config file
    //m_variables.Print();
    m_data = &data; //assigning transient data pointer

    // Load variables from config file
    m_variables.Get( "verbosity_SecondaryLeptonLikelihoodReco", m_verbosity_SecondaryLeptonLikelihoodReco );
    if( !verbostty_check( m_verbosity_SecondaryLeptonLikelihoodReco ) ) {
        Log( "SecondaryLeptonLikelihoodReco Tool<ERROR>: Invalid value for `verbosity_SecondaryLeptonLikelihoodReco`: " + to_string( m_verbosity_SecondaryLeptonLikelihoodReco ), m_verbosity_error, 99999 ); 
        return false;
    }
    m_variables.Get( "verbosity_DetectorResponsePredictor", m_verbosity_DetectorResponsePredictor );
    if( !verbostty_check( m_verbosity_DetectorResponsePredictor ) ) {
        Log( "SecondaryLeptonLikelihoodReco Tool: Invalid value for `verbosity_DetectorResponsePredictor`: " + to_string( m_verbosity_DetectorResponsePredictor ), m_verbosity_error, 99999 ); 
        return false;
    }
    
    m_variables.Get( "hists_emission_mu_water_path"        , m_hists_emission_mu_water_path         );
    m_variables.Get( "hists_emission_e_water_path"         , m_hists_emission_e_water_path          );
    m_variables.Get( "hists_emission_mu_sci_path"          , m_hists_emission_mu_sci_path           );
    m_variables.Get( "hists_emission_e_sci_path"           , m_hists_emission_e_sci_path            );
    
    m_variables.Get( "hists_emission_mu_water_energy_min"  , m_hists_emission_mu_water_energy_min   );
    m_variables.Get( "hists_emission_e_water_energy_min"   , m_hists_emission_e_water_energy_min    );
    m_variables.Get( "hists_emission_mu_sci_energy_min"    , m_hists_emission_mu_sci_energy_min     );
    m_variables.Get( "hists_emission_e_sci_energy_min"     , m_hists_emission_e_sci_energy_min      );
                                                             
    m_variables.Get( "hists_emission_mu_water_energy_delta", m_hists_emission_mu_water_energy_delta );
    m_variables.Get( "hists_emission_e_water_energy_delta" , m_hists_emission_e_water_energy_delta  );
    m_variables.Get( "hists_emission_mu_sci_energy_delta"  , m_hists_emission_mu_sci_energy_delta   );
    m_variables.Get( "hists_emission_e_sci_energy_delta"   , m_hists_emission_e_sci_energy_delta    );
                                                             
    m_variables.Get( "hists_emission_mu_water_num"         , m_hists_emission_mu_water_num          );
    m_variables.Get( "hists_emission_e_water_num"          , m_hists_emission_e_water_num           );
    m_variables.Get( "hists_emission_mu_sci_num"           , m_hists_emission_mu_sci_num            );
    m_variables.Get( "hists_emission_e_sci_num"            , m_hists_emission_e_sci_num             );
                                                             
    m_variables.Get( "hists_emission_mu_water_name"        , m_hists_emission_mu_water_name         );
    m_variables.Get( "hists_emission_e_water_name"         , m_hists_emission_e_water_name          );
    m_variables.Get( "hists_emission_mu_sci_name"          , m_hists_emission_mu_sci_name           );
    m_variables.Get( "hists_emission_e_sci_name"           , m_hists_emission_e_sci_name            );
                                                             
    m_variables.Get( "hists_dEdX_mu_water_path"            , m_hists_dEdX_mu_water_path             );
    m_variables.Get( "hists_dEdX_e_water_path"             , m_hists_dEdX_e_water_path              );
    m_variables.Get( "hists_dEdX_mu_sci_path"              , m_hists_dEdX_mu_sci_path               );
    m_variables.Get( "hists_dEdX_e_sci_path"               , m_hists_dEdX_e_sci_path                );
                                                             
    m_variables.Get( "hists_dEdX_mu_water_path"            , m_hists_dEdX_mu_water_path             );
    m_variables.Get( "hists_dEdX_e_water_path"             , m_hists_dEdX_e_water_path              );
    m_variables.Get( "hists_dEdX_mu_sci_path"              , m_hists_dEdX_mu_sci_path               );
    m_variables.Get( "hists_dEdX_e_sci_path"               , m_hists_dEdX_e_sci_path                );
                                                             
    m_variables.Get( "hists_dEdX_mu_water_name"            , m_hists_dEdX_mu_water_name             );
    m_variables.Get( "hists_dEdX_e_water_name"             , m_hists_dEdX_e_water_name              );
    m_variables.Get( "hists_dEdX_mu_sci_name"              , m_hists_dEdX_mu_sci_name               );
    m_variables.Get( "hists_dEdX_e_sci_name"               , m_hists_dEdX_e_sci_name                );

    // Load digits
    if( !m_data->Stores.at( "RecoEvent" )->Get( "RecoDigit", m_digits ) ){
        Log( "SecondaryLeptonLikelihoodReco Tool: Error retrieving RecoDigits, no digit from the RecoEvent!", m_verbosity_error, m_verbosity ); 
        return false;
    }
    m_vtxGeo->LoadDigits( m_digits );

    // Initialize detector response predictor

    m_LRLC->reset_members();
    m_LRLC->set_verbosity( m_verbosity );
    m_LRLC->set_vtxGeo( m_vtxGeo );
    //m_LRLC->load_hists_emission_mu_water(

    return true;
}


bool SecondaryLeptonLikelihoodReco::Execute()
{
  Log( "===========================================================================================", m_verbosity_debug, m_verbosity);
  Log( "SecondaryLeptonLikelihoodReco Tool: Executing", m_verbosity_debug, m_verbosity);

  return true;
}


bool SecondaryLeptonLikelihoodReco::Finalise()
{
  return true;
}

bool SecondaryLeptonLikelihoodReco::load_hists( string t_hists_path, void ( *loadingFunction )( vector< string >, vector< int >, vector< string > ) ) {
  vector< string > hists_paths;
  vector< int    > hists_IDs;
  vector< string > hists_names;

  if( !exists( t_hists_path ) ) {
    string temp_string = "SecondaryLeptonLikelihoodReco Tool: Error " + t_hists_path + " does not exist!";
    Log( temp_string.c_str(), m_verbosity_error, m_verbosity );
    return false;
  }
  for( directory_entry& entry : directory_iterator( t_hists_path ) ) {
    hists_paths.push_back( entry.path().root_path().generic_string() );
    hists_IDs.push_back( stoi( entry.path().stem().generic_string() ) );
    hists_paths.push_back( ";1" );
  }

  loadingFunction( hists_paths, hists_IDs, hists_names );

  return true;
}

inline bool SecondaryLeptonLikelihoodReco::verbosity_check( const unsigned int t_verbosity ) const {
    if( t_verbosity <= 3 ) 
        return true;
    return false;
}

inline bool SecondaryLeptonLikelihoodReco::path_check( const string& t_path ) const {
    if( !exists( t_path ) )
        return false;
    return true;
}

inline void SecondaryLeptonLikelihoodReco::Log_debug( const string& t_message, const unsigned int t_verbosity ) const {
    if( t_verbosity >= m_verbosity_SecondaryLeptonLikelihoodReco )
        cout << "SecondaryLeptonLikelihoodReco || " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message;
}

inline void SecondaryLeptonLikelihoodReco::Log_debug( const string&& t_message, const unsigned int t_verbosity ) const {
    if( t_verbosity >= m_verbosity_SecondaryLeptonLikelihoodReco )
        cout << "SecondaryLeptonLikelihoodReco || " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message;
}