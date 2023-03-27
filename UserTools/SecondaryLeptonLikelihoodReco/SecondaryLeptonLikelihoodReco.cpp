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
    m_variables.Get( "verbosity"                  , m_verbosity                   );
    m_variables.Get( "hists_emission_mu_water_dir", m_hists_emission_mu_water_dir );
    m_variables.Get( "hists_emission_e_water_dir" , m_hists_emission_e_water_dir  );
    m_variables.Get( "hists_emission_mu_sci_dir"  , m_hists_emission_mu_sci_dir   );
    m_variables.Get( "hists_emission_e_sci_dir"   , m_hists_emission_e_sci_dir    );

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

bool SecondaryLeptonLikelihoodReco::load_hists( string t_hists_dir, void ( *loadingFunction )( vector< string >, vector< int >, vector< string > ) ) {
  vector< string > hists_paths;
  vector< int    > hists_IDs;
  vector< string > hists_names;

  if( !exists( t_hists_dir ) ) {
    string temp_string = "SecondaryLeptonLikelihoodReco Tool: Error " + t_hists_dir + "does not exist!";
    Log( temp_string.c_str(), m_verbosity_error, m_verbosity );
    return false;
  }
  for( directory_entry& entry : directory_iterator( t_hists_dir ) ) {
    hists_paths.push_back( entry.path().root_path().generic_string() );
    hists_IDs.push_back( stoi( entry.path().stem().generic_string() ) );
    hists_paths.push_back( ";1" );
  }

  loadingFunction( hists_paths, hists_IDs, hists_names );

  return true;
}
