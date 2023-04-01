#include "SecondaryLeptonLikelihoodReco.h"

#include "boost/filesystem.hpp"

#include <functional> 

using boost::filesystem::exists;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;
using boost::filesystem::is_regular_file;

using std::bind;
using std::function;

bool SecondaryLeptonLikelihoodReco::Initialise( string configfile, DataModel& data )
{
    // Load config file
    if( configfile != "" ) m_variables.Initialise( configfile ); // loading config file
    //m_variables.Print();
    m_data = &data; //assigning transient data pointer

    // Load variables from config file
    if( !get_config_verbosity  ( "verbosity_SecondaryLeptonLikelihoodReco" , m_verbosity_SecondaryLeptonLikelihoodReco  ) ) return false;
    if( !get_config_verbosity  ( "verbosity_DetectorResponsePredictor"     , m_verbosity_DetectorResponsePredictor      ) ) return false;
    
    if( !get_config_path       ( "hists_emission_mu_tankWater_path"        , m_hists_emission_mu_tankWater_path         ) ) return false;
    if( !get_config_path       ( "hists_emission_e_tankWater_path"         , m_hists_emission_e_tankWater_path          ) ) return false;
    if( !get_config_path       ( "hists_emission_mu_MRDsci_path"           , m_hists_emission_mu_MRDsci_path            ) ) return false;
    if( !get_config_path       ( "hists_emission_e_MRDsci_path"            , m_hists_emission_e_MRDsci_path             ) ) return false;
    
    if( !get_config_unsignedInt( "hists_emission_mu_tankWater_energy_min"  , m_hists_emission_mu_tankWater_energy_min   ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_e_tankWater_energy_min"   , m_hists_emission_e_tankWater_energy_min    ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_mu_MRDsci_energy_min"     , m_hists_emission_mu_MRDsci_energy_min      ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_e_MRDsci_energy_min"      , m_hists_emission_e_MRDsci_energy_min       ) ) return false;

    if( !get_config_unsignedInt( "hists_emission_mu_tankWater_energy_delta", m_hists_emission_mu_tankWater_energy_delta ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_e_tankWater_energy_delta" , m_hists_emission_e_tankWater_energy_delta  ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_mu_MRDsci_energy_delta"   , m_hists_emission_mu_MRDsci_energy_delta    ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_e_MRDsci_energy_delta"    , m_hists_emission_e_MRDsci_energy_delta     ) ) return false;

    if( !get_config_unsignedInt( "hists_emission_mu_tankWater_num"         , m_hists_emission_mu_tankWater_num          ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_e_tankWater_num"          , m_hists_emission_e_tankWater_num           ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_mu_MRDsci_num"            , m_hists_emission_mu_MRDsci_num             ) ) return false;
    if( !get_config_unsignedInt( "hists_emission_e_MRDsci_num"             , m_hists_emission_e_MRDsci_num              ) ) return false;

    if( !get_config_histName   ( "hists_emission_mu_tankWater_name"        , m_hists_emission_mu_tankWater_name         ) ) return false;
    if( !get_config_histName   ( "hists_emission_e_tankWater_name"         , m_hists_emission_e_tankWater_name          ) ) return false;
    if( !get_config_histName   ( "hists_emission_mu_MRDsci_name"           , m_hists_emission_mu_MRDsci_name            ) ) return false;
    if( !get_config_histName   ( "hists_emission_e_MRDsci_name"            , m_hists_emission_e_MRDsci_name             ) ) return false;

    if( !get_config_path       ( "hists_dEdX_mu_tankWater_path"            , m_hists_dEdX_mu_tankWater_path             ) ) return false;
    if( !get_config_path       ( "hists_dEdX_e_tankWater_path"             , m_hists_dEdX_e_tankWater_path              ) ) return false;
    if( !get_config_path       ( "hists_dEdX_mu_tankSteel_path"            , m_hists_dEdX_mu_tankSteel_path             ) ) return false;
    if( !get_config_path       ( "hists_dEdX_e_tankSteel_path"             , m_hists_dEdX_e_tankSteel_path              ) ) return false;
    if( !get_config_path       ( "hists_dEdX_mu_MRDsci_path"               , m_hists_dEdX_mu_MRDsci_path                ) ) return false;
    if( !get_config_path       ( "hists_dEdX_e_MRDsci_path"                , m_hists_dEdX_e_MRDsci_path                 ) ) return false;
    if( !get_config_path       ( "hists_dEdX_mu_MRDiron_path"              , m_hists_dEdX_mu_MRDiron_path               ) ) return false;
    if( !get_config_path       ( "hists_dEdX_e_MRDiron_path"               , m_hists_dEdX_e_MRDiron_path                ) ) return false;

    if( !get_config_histName   ( "hists_dEdX_mu_tankWater_name"            , m_hists_dEdX_mu_tankWater_name             ) ) return false;
    if( !get_config_histName   ( "hists_dEdX_e_tankWater_name"             , m_hists_dEdX_e_tankWater_name              ) ) return false;
    if( !get_config_histName   ( "hists_dEdX_mu_tankSteel_name"            , m_hists_dEdX_mu_tankWater_name             ) ) return false;
    if( !get_config_histName   ( "hists_dEdX_e_tankSteel_name"             , m_hists_dEdX_e_tankWater_name              ) ) return false;
    if( !get_config_histName   ( "hists_dEdX_mu_MRDsci_name"               , m_hists_dEdX_mu_MRDsci_name                ) ) return false;
    if( !get_config_histName   ( "hists_dEdX_e_MRDsci_name"                , m_hists_dEdX_e_MRDsci_name                 ) ) return false;
    if( !get_config_histName   ( "hists_dEdX_mu_MRDiron_name"              , m_hists_dEdX_mu_MRDiron_name               ) ) return false;
    if( !get_config_histName   ( "hists_dEdX_e_MRDiron_name"               , m_hists_dEdX_e_MRDiron_name                ) ) return false;

    if( !get_config_path       ( "hists_transmittance_mu_tankWater_path"   , m_hists_transmittance_mu_tankWater_path    ) ) return false;
    if( !get_config_path       ( "hists_transmittance_e_tankWater_path"    , m_hists_transmittance_e_tankWater_path     ) ) return false;
    if( !get_config_path       ( "hists_transmittance_mu_MRDsci_path"      , m_hists_transmittance_mu_MRDsci_path       ) ) return false;
    if( !get_config_path       ( "hists_transmittance_e_MRDsci_path"       , m_hists_transmittance_e_MRDsci_path        ) ) return false;

    if( !get_config_histName   ( "hists_transmittance_mu_tankWater_name"   , m_hists_transmittance_mu_tankWater_name    ) ) return false;
    if( !get_config_histName   ( "hists_transmittance_e_tankWater_name"    , m_hists_transmittance_e_tankWater_name     ) ) return false;
    if( !get_config_histName   ( "hists_transmittance_mu_MRDsci_name"      , m_hists_transmittance_mu_MRDsci_name       ) ) return false;
    if( !get_config_histName   ( "hists_transmittance_e_MRDsci_name"       , m_hists_transmittance_e_MRDsci_name        ) ) return false;

    // Load digits
    if( !m_data->Stores.at( "RecoEvent" )->Get( "RecoDigit", m_digits ) ){
        string temp_string{ "Cannot retrieve RecoDigits. No digit from the RecoEvent!" };
        Log_debug( temp_string, m_verbosity_error ); 
        return false;
    }
    m_vtxGeo->LoadDigits( m_digits );

    // Initialize DetectorResponsePredictor
    const unsigned int num_materials{ 2 };
    const unsigned int num_particles{ 2 };
    const unsigned int hists_emission_energy_mins  [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_energy_min  , m_hists_emission_e_tankWater_energy_min   }, 
                                                                                       { m_hists_emission_mu_MRDsci_energy_min     , m_hists_emission_e_MRDsci_energy_min      } };
    const unsigned int hists_emission_energy_deltas[ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_energy_delta, m_hists_emission_e_tankWater_energy_delta }, 
                                                                                       { m_hists_emission_mu_MRDsci_energy_delta   , m_hists_emission_e_MRDsci_energy_delta    } };
    const unsigned int hists_emission_energy_nums  [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_num         , m_hists_emission_e_tankWater_num          }, 
                                                                                       { m_hists_emission_mu_MRDsci_num            , m_hists_emission_e_MRDsci_num             } };
    const string       hists_emission_paths        [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_path        , m_hists_emission_e_tankWater_path         }, 
                                                                                       { m_hists_emission_mu_MRDsci_path           , m_hists_emission_e_MRDsci_path            } };
    const string       hists_emission_names        [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_name        , m_hists_emission_e_tankWater_name         }, 
                                                                                       { m_hists_emission_mu_MRDsci_name           , m_hists_emission_e_MRDsci_name            } };
    using DetectorResponsePredictorFunctionPtr = bool ( DetectorResponsePredictor::* )( const vector< string >&, const vector< int >&, const vector< string >& );
    DetectorResponsePredictorFunctionPtr load_hists_emissions[ num_materials ][ 2 ]{ { &DetectorResponsePredictor::load_hists_emission_tankWater, &DetectorResponsePredictor::load_hists_emission_tankWater_energies },
                                                                                     { &DetectorResponsePredictor::load_hists_emission_MRDsci   , &DetectorResponsePredictor::load_hists_emission_MRDsci_energies    } };
    DetectorResponsePredictor* DetectorResponsePredictors[ num_particles ]{ m_DetectorResponsePredictor_mu, m_DetectorResponsePredictor_e };
    unsigned int     hists_emission_energy_cur;
    vector< string > hists_emission_paths_cur ;
    vector< int    > hists_emission_IDs_cur   ;
    vector< string > hists_emission_names_cur ;
    for( unsigned int nMaterial{ 0 }; nMaterial < num_materials; nMaterial++ )
        for( unsigned int nParticle{ 0 }; nParticle < num_particles; nParticle++ ) {
            hists_emission_energy_cur = hists_emission_energy_mins[ nMaterial ][ nParticle ];
            hists_emission_paths_cur.empty();
            hists_emission_IDs_cur  .empty();
            hists_emission_names_cur.empty();
            for( unsigned int nEnergy{ 0 }; nEnergy < hists_emission_energy_nums[ nMaterial ][ nParticle ]; nEnergy++ ) {
                hists_emission_paths_cur.push_back( hists_emission_paths[ nMaterial ][ nParticle ] );
                hists_emission_IDs_cur  .push_back( hists_emission_energy_cur                      );
                hists_emission_names_cur.push_back( hists_emission_names[ nMaterial ][ nParticle ] );
                hists_emission_energy_cur += hists_emission_energy_deltas[ nMaterial ][ nParticle ];
            }
            for( unsigned int i{ 0 }; i < 2; i++ )
                if( !( DetectorResponsePredictors[ nParticle ]->*load_hists_emissions[ nMaterial ][ i ] )( hists_emission_paths_cur, hists_emission_IDs_cur, hists_emission_names_cur ) ) {
                    string temp_string{ "Cannot load emission histograms (nMaterial=" };
                    Log_debug( temp_string + to_string( nMaterial ) + ", nParticle=" + to_string( nParticle ) + " and i=" + to_string( i ) + ").", m_verbosity_error );
                    return false;
                }
        }

    const string hist_transmittance_tankWater_paths[ num_particles ]{ m_hists_transmittance_mu_tankWater_path, m_hists_transmittance_e_tankWater_path };
    const string hist_transmittance_MRDsci_paths   [ num_particles ]{ m_hists_transmittance_mu_MRDsci_path   , m_hists_transmittance_e_MRDsci_path    };
    const string hist_dEdX_tankWater_paths         [ num_particles ]{ m_hists_dEdX_mu_tankWater_path         , m_hists_dEdX_e_tankWater_path          };
    const string hist_dEdX_tankSteel_paths         [ num_particles ]{ m_hists_dEdX_mu_tankSteel_path         , m_hists_dEdX_e_tankSteel_path          };
    const string hist_dEdX_MRDsci_paths            [ num_particles ]{ m_hists_dEdX_mu_MRDsci_path            , m_hists_dEdX_e_MRDsci_path             };
    const string hist_dEdX_MRDiron_paths           [ num_particles ]{ m_hists_dEdX_mu_MRDiron_path           , m_hists_dEdX_e_MRDiron_path            };
    
    const string hist_transmittance_tankWater_names[ num_particles ]{ m_hists_transmittance_mu_tankWater_name, m_hists_transmittance_e_tankWater_name };
    const string hist_transmittance_MRDsci_names   [ num_particles ]{ m_hists_transmittance_mu_MRDsci_name   , m_hists_transmittance_e_MRDsci_name    };
    const string hist_dEdX_tankWater_names         [ num_particles ]{ m_hists_dEdX_mu_tankWater_name         , m_hists_dEdX_e_tankWater_name          };
    const string hist_dEdX_tankSteel_names         [ num_particles ]{ m_hists_dEdX_mu_tankSteel_name         , m_hists_dEdX_e_tankSteel_name          };
    const string hist_dEdX_MRDsci_names            [ num_particles ]{ m_hists_dEdX_mu_MRDsci_name            , m_hists_dEdX_e_MRDsci_name             };
    const string hist_dEdX_MRDiron_names           [ num_particles ]{ m_hists_dEdX_mu_MRDiron_name           , m_hists_dEdX_e_MRDiron_name            };

    for( unsigned int nParticle{ 0 }; nParticle < num_particles; nParticle++ ) {
        DetectorResponsePredictors[ nParticle ]->load_hist_transmission_tankWater( hist_transmittance_tankWater_paths[ nParticle ], hist_transmittance_tankWater_names[ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_transmission_MRDsci   ( hist_transmittance_MRDsci_paths   [ nParticle ], hist_transmittance_MRDsci_names   [ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_tankWater        ( hist_dEdX_tankWater_paths         [ nParticle ], hist_dEdX_tankWater_names         [ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_tankSteel        ( hist_dEdX_tankSteel_paths         [ nParticle ], hist_dEdX_tankSteel_names         [ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_MRDsci           ( hist_dEdX_MRDsci_paths            [ nParticle ], hist_dEdX_MRDsci_names            [ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_MRDiron          ( hist_dEdX_MRDiron_paths           [ nParticle ], hist_dEdX_MRDiron_names           [ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->set_verbosity                   ( m_verbosity_DetectorResponsePredictor                                                            );
    }

    return true;
}


bool SecondaryLeptonLikelihoodReco::Execute()
{
  Log( "===========================================================================================", m_verbosity_debug, m_verbosity_SecondaryLeptonLikelihoodReco );
  Log( "SecondaryLeptonLikelihoodReco Tool: Executing", m_verbosity_debug, m_verbosity_SecondaryLeptonLikelihoodReco );

  return true;
}


bool SecondaryLeptonLikelihoodReco::Finalise()
{
    delete m_DetectorResponsePredictor_mu;
    delete m_DetectorResponsePredictor_e ;

    return true;
}

inline void SecondaryLeptonLikelihoodReco::Log_debug( const string& t_message, unsigned int t_verbosity ) {
    if( t_verbosity >= m_verbosity_SecondaryLeptonLikelihoodReco )
        cout << "UserTool_SecondaryLeptonLikelihoodReco || " << m_verbosity_map[ t_verbosity ] << " || " 
             << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message << endl;
}

inline void SecondaryLeptonLikelihoodReco::Log_debug( const string&& t_message, unsigned int t_verbosity ) {
    if( t_verbosity >= m_verbosity_SecondaryLeptonLikelihoodReco )
        cout << "UserTool_SecondaryLeptonLikelihoodReco || " << m_verbosity_map[ t_verbosity ] << " || " 
             << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << "): " << t_message << endl;
}

inline bool SecondaryLeptonLikelihoodReco::get_config_path( const string& t_variable_name, string& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load path variable `" };
        Log_debug( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    } else if( !exists( t_variable ) ) {
        string temp_string{ "Invalid path for `" };
        Log_debug( temp_string + t_variable_name + "`: \"" + t_variable + "\". Path does not exist.", m_verbosity_error );
        return false;
    } else if( !is_regular_file( t_variable ) ) {
        string temp_string{ "Invalid path for `" };
        Log_debug( temp_string + t_variable_name + "`: \"" + t_variable + "\". Path is not a \"regular file\" (cannot be a directory).", m_verbosity_error );
        return false;
    }
    return true;
}

inline bool SecondaryLeptonLikelihoodReco::get_config_verbosity( const string& t_variable_name, unsigned int& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load verbosity variable `" };
        Log_debug( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    } else if( t_variable > 3 ) {
        string temp_string{ "Invalid verbosity value for `" };
        Log_debug( temp_string + t_variable_name + "`: \"" + to_string( t_variable ) + "\". Must be 3 or less (0=error, 1=warining, 2=message, and 3=debug).", m_verbosity_error );
        return false;
    }
    return true;
}

inline bool SecondaryLeptonLikelihoodReco::get_config_histName( const string& t_variable_name, string& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load histogram name variable `" };
        Log_debug( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    } else if( t_variable.find( ';' ) != string::npos ) {
        string temp_string{ "Invalid histogram name for `" };
        Log_debug( temp_string + t_variable_name + "`: \"" + t_variable + "\". Must contain ';' (e.g. `histName;1`).", m_verbosity_error );
        return false;
    }
    return true;
}

inline bool SecondaryLeptonLikelihoodReco::get_config_unsignedInt( const string& t_variable_name, unsigned int& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load unsigned int `" };
        Log_debug( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    }
    return true;
}

inline bool SecondaryLeptonLikelihoodReco::get_config_double( const string& t_variable_name, double& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load double `" };
        Log_debug( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    }
    return true;
}