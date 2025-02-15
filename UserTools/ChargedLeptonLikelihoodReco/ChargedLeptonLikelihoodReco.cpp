#include "ChargedLeptonLikelihoodReco.h"

#include "boost/filesystem.hpp"

#include <functional> 

using boost::filesystem::exists;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;
using boost::filesystem::is_regular_file;

using std::bind;
using std::function;

bool ChargedLeptonLikelihoodReco::Initialise( string configfile, DataModel& data )
{
    // Load config file
    if( configfile != "" ) m_variables.Initialise( configfile ); // loading config file
    //m_variables.Print();
    m_data = &data; //assigning transient data pointer

    // Load variables from config file
    if( !get_config_verbosity  ( "verbosity_ChargedLeptonLikelihoodReco"      , m_verbosity_ChargedLeptonLikelihoodReco       ) ) return false;
    if( !get_config_verbosity  ( "verbosity_DetectorResponsePredictor"        , m_verbosity_DetectorResponsePredictor         ) ) return false;
    if( !get_config_verbosity  ( "verbosity_THistReader"                      , m_verbosity_THistReader                       ) ) return false;
                                                                                                                                             
    if( !get_config_path       ( "hists_emission_mu_tankWater_energies_path"  , m_hists_emission_mu_tankWater_energies_path   ) ) return false;
    if( !get_config_path       ( "hists_emission_e_tankWater_energies_path"   , m_hists_emission_e_tankWater_energies_path    ) ) return false;
    if( !get_config_path       ( "hists_emission_mu_MRDsci_energies_path"     , m_hists_emission_mu_MRDsci_energies_path      ) ) return false;
    if( !get_config_path       ( "hists_emission_e_MRDsci_energies_path"      , m_hists_emission_e_MRDsci_energies_path       ) ) return false;
                                                                                                                                             
    if( !get_config_path       ( "hists_emission_mu_tankWater_counts_path"    , m_hists_emission_mu_tankWater_counts_path     ) ) return false;
    if( !get_config_path       ( "hists_emission_e_tankWater_counts_path"     , m_hists_emission_e_tankWater_counts_path      ) ) return false;
    if( !get_config_path       ( "hists_emission_mu_MRDsci_counts_path"       , m_hists_emission_mu_MRDsci_counts_path        ) ) return false;
    if( !get_config_path       ( "hists_emission_e_MRDsci_counts_path"        , m_hists_emission_e_MRDsci_counts_path         ) ) return false;
                                                                                                                                             
    if( !get_config_histName   ( "hists_emission_mu_tankWater_energies_name"  , m_hists_emission_mu_tankWater_energies_name   ) ) return false;
    if( !get_config_histName   ( "hists_emission_e_tankWater_energies_name"   , m_hists_emission_e_tankWater_energies_name    ) ) return false;
    if( !get_config_histName   ( "hists_emission_mu_MRDsci_energies_name"     , m_hists_emission_mu_MRDsci_energies_name      ) ) return false;
    if( !get_config_histName   ( "hists_emission_e_MRDsci_energies_name"      , m_hists_emission_e_MRDsci_energies_name       ) ) return false;
                                                                                                                                             
    if( !get_config_histName   ( "hists_emission_mu_tankWater_counts_name"    , m_hists_emission_mu_tankWater_counts_name     ) ) return false;
    if( !get_config_histName   ( "hists_emission_e_tankWater_counts_name"     , m_hists_emission_e_tankWater_counts_name      ) ) return false;
    if( !get_config_histName   ( "hists_emission_mu_MRDsci_counts_name"       , m_hists_emission_mu_MRDsci_counts_name        ) ) return false;
    if( !get_config_histName   ( "hists_emission_e_MRDsci_counts_name"        , m_hists_emission_e_MRDsci_counts_name         ) ) return false;

    // if( !get_config_unit_energy( "hists_emission_mu_tankWater_energy_unit"    , m_hists_emission_mu_tankWater_energy_unit     ) ) return false;
    // if( !get_config_unit_energy( "hists_emission_e_tankWater_energy_unit"     , m_hists_emission_e_tankWater_energy_unit      ) ) return false;
    // if( !get_config_unit_energy( "hists_emission_mu_MRDsci_energy_unit"       , m_hists_emission_mu_MRDsci_energy_unit        ) ) return false;
    // if( !get_config_unit_energy( "hists_emission_e_MRDsci_energy_unit"        , m_hists_emission_e_MRDsci_energy_unit         ) ) return false;
                                                                                                                                             
    // if( !get_config_unsignedInt( "hists_emission_mu_tankWater_energy_min"     , m_hists_emission_mu_tankWater_energy_min      ) ) return false; 
    // if( !get_config_unsignedInt( "hists_emission_e_tankWater_energy_min"      , m_hists_emission_e_tankWater_energy_min       ) ) return false; 
    // if( !get_config_unsignedInt( "hists_emission_mu_MRDsci_energy_min"        , m_hists_emission_mu_MRDsci_energy_min         ) ) return false; 
    // if( !get_config_unsignedInt( "hists_emission_e_MRDsci_energy_min"         , m_hists_emission_e_MRDsci_energy_min          ) ) return false; 
                                                                                                                                              
    // if( !get_config_unsignedInt( "hists_emission_mu_tankWater_energy_delta"   , m_hists_emission_mu_tankWater_energy_delta    ) ) return false; 
    // if( !get_config_unsignedInt( "hists_emission_e_tankWater_energy_delta"    , m_hists_emission_e_tankWater_energy_delta     ) ) return false; 
    // if( !get_config_unsignedInt( "hists_emission_mu_MRDsci_energy_delta"      , m_hists_emission_mu_MRDsci_energy_delta       ) ) return false; 
    // if( !get_config_unsignedInt( "hists_emission_e_MRDsci_energy_delta"       , m_hists_emission_e_MRDsci_energy_delta        ) ) return false; 

    if( !get_config_measurement( "hists_emission_mu_tankWater_energy_min"        ,
                                 "hists_emission_mu_tankWater_energy_min_unit"   , m_hists_emission_mu_tankWater_energy_min        ) ) return false;
    if( !get_config_measurement( "hists_emission_e_tankWater_energy_min"         ,
                                 "hists_emission_e_tankWater_energy_min_unit"    , m_hists_emission_e_tankWater_energy_min         ) ) return false;
    if( !get_config_measurement( "hists_emission_mu_MRDsci_energy_min"           ,
                                 "hists_emission_mu_MRDsci_energy_min_unit"      , m_hists_emission_mu_MRDsci_energy_min           ) ) return false;
    if( !get_config_measurement( "hists_emission_e_MRDsci_energy_min"            ,
                                 "hists_emission_e_MRDsci_energy_min_unit"       , m_hists_emission_e_MRDsci_energy_min            ) ) return false;

    if( !get_config_measurement( "hists_emission_mu_tankWater_energy_delta"      ,
                                 "hists_emission_mu_tankWater_energy_delta_unit" , m_hists_emission_mu_tankWater_energy_delta      ) ) return false;
    if( !get_config_measurement( "hists_emission_e_tankWater_energy_delta"       ,
                                 "hists_emission_e_tankWater_energy_delta_unit"  , m_hists_emission_e_tankWater_energy_delta       ) ) return false;
    if( !get_config_measurement( "hists_emission_mu_MRDsci_energy_delta"         ,
                                 "hists_emission_mu_MRDsci_energy_delta_unit"    , m_hists_emission_mu_MRDsci_energy_delta         ) ) return false;
    if( !get_config_measurement( "hists_emission_e_MRDsci_energy_delta"          ,
                                 "hists_emission_e_MRDsci_energy_delta_unit"     , m_hists_emission_e_MRDsci_energy_delta          ) ) return false;
                                                                                                                                              
    if( !get_config_unsignedInt( "hists_emission_mu_tankWater_num"            , m_hists_emission_mu_tankWater_num             ) ) return false; 
    if( !get_config_unsignedInt( "hists_emission_e_tankWater_num"             , m_hists_emission_e_tankWater_num              ) ) return false; 
    if( !get_config_unsignedInt( "hists_emission_mu_MRDsci_num"               , m_hists_emission_mu_MRDsci_num                ) ) return false; 
    if( !get_config_unsignedInt( "hists_emission_e_MRDsci_num"                , m_hists_emission_e_MRDsci_num                 ) ) return false; 
                                                                                                                                              
    if( !get_config_path       ( "hists_dEdX_mu_tankWater_path"               , m_hists_dEdX_mu_tankWater_path                ) ) return false; 
    if( !get_config_path       ( "hists_dEdX_e_tankWater_path"                , m_hists_dEdX_e_tankWater_path                 ) ) return false; 
    if( !get_config_path       ( "hists_dEdX_mu_tankSteel_path"               , m_hists_dEdX_mu_tankSteel_path                ) ) return false; 
    if( !get_config_path       ( "hists_dEdX_e_tankSteel_path"                , m_hists_dEdX_e_tankSteel_path                 ) ) return false; 
    if( !get_config_path       ( "hists_dEdX_mu_MRDsci_path"                  , m_hists_dEdX_mu_MRDsci_path                   ) ) return false; 
    if( !get_config_path       ( "hists_dEdX_e_MRDsci_path"                   , m_hists_dEdX_e_MRDsci_path                    ) ) return false; 
    if( !get_config_path       ( "hists_dEdX_mu_MRDiron_path"                 , m_hists_dEdX_mu_MRDiron_path                  ) ) return false; 
    if( !get_config_path       ( "hists_dEdX_e_MRDiron_path"                  , m_hists_dEdX_e_MRDiron_path                   ) ) return false; 
                                                                                                                                              
    if( !get_config_histName   ( "hists_dEdX_mu_tankWater_name"               , m_hists_dEdX_mu_tankWater_name                ) ) return false; 
    if( !get_config_histName   ( "hists_dEdX_e_tankWater_name"                , m_hists_dEdX_e_tankWater_name                 ) ) return false; 
    if( !get_config_histName   ( "hists_dEdX_mu_tankSteel_name"               , m_hists_dEdX_mu_tankWater_name                ) ) return false; 
    if( !get_config_histName   ( "hists_dEdX_e_tankSteel_name"                , m_hists_dEdX_e_tankWater_name                 ) ) return false; 
    if( !get_config_histName   ( "hists_dEdX_mu_MRDsci_name"                  , m_hists_dEdX_mu_MRDsci_name                   ) ) return false; 
    if( !get_config_histName   ( "hists_dEdX_e_MRDsci_name"                   , m_hists_dEdX_e_MRDsci_name                    ) ) return false; 
    if( !get_config_histName   ( "hists_dEdX_mu_MRDiron_name"                 , m_hists_dEdX_mu_MRDiron_name                  ) ) return false; 
    if( !get_config_histName   ( "hists_dEdX_e_MRDiron_name"                  , m_hists_dEdX_e_MRDiron_name                   ) ) return false; 

    if( !get_config_bool       ( "useSingleTH1forPhotonTransmission_tankWater", m_useSingleTH1forPhotonTransmission_tankWater ) ) return false;
    if( !get_config_bool       ( "useSingleTH1forPhotonTransmission_MRDsci"   , m_useSingleTH1forPhotonTransmission_MRDsci    ) ) return false;
                                                                                                                                                    
    if( !get_config_path       ( "hists_transmission_tankWater_path"          , m_hists_transmission_tankWater_path           ) ) return false; 
    if( !get_config_path       ( "hists_transmission_MRDsci_path"             , m_hists_transmission_MRDsci_path              ) ) return false; 
                                                                                                                                                  
    if( !get_config_histName   ( "hists_transmission_tankWater_name"          , m_hists_transmission_tankWater_name           ) ) return false; 
    if( !get_config_histName   ( "hists_transmission_MRDsci_name"             , m_hists_transmission_MRDsci_name              ) ) return false; 

    if( !get_config_measurement( "hists_transmission_tankWater_energy_min"        ,
                                 "hists_transmission_tankWater_energy_min_unit"   , m_hists_transmission_tankWater_energy_min        ) ) return false;
    if( !get_config_measurement( "hists_transmission_MRDsci_energy_min"           ,
                                 "hists_transmission_MRDsci_energy_min_unit"      , m_hists_transmission_MRDsci_energy_min           ) ) return false;

    if( !get_config_measurement( "hists_transmission_tankWater_energy_delta"      ,
                                 "hists_transmission_tankWater_energy_delta_unit" , m_hists_transmission_tankWater_energy_delta      ) ) return false;
    if( !get_config_measurement( "hists_transmission_MRDsci_energy_delta"         ,
                                 "hists_transmission_MRDsci_energy_delta_unit"    , m_hists_transmission_MRDsci_energy_delta         ) ) return false;
                                                                                                                              
    if( !get_config_unsignedInt( "hists_transmission_tankWater_energy_num"    , m_hists_transmission_tankWater_num            ) ) return false;
    if( !get_config_unsignedInt( "hists_transmission_MRDsci_energy_num"       , m_hists_transmission_MRDsci_num               ) ) return false;



    // Initialize DetectorResponsePredictor
    const unsigned int num_materials{ 2 };
    const unsigned int num_particles{ 2 };
    string materialNames[ num_materials ]{ "tankWater", "MRDsci" };
    string DetectorResponsePredictors_particles[ num_particles ]{ "mu", "e" };
    DetectorResponsePredictor* DetectorResponsePredictors[ num_particles ]{ m_DetectorResponsePredictor_mu, m_DetectorResponsePredictor_e };

    const string       hists_emission_energies_paths[ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_energies_path, m_hists_emission_e_tankWater_energies_path }, 
                                                                                        { m_hists_emission_mu_MRDsci_energies_path   , m_hists_emission_e_MRDsci_energies_path    } };
    const string       hists_emission_counts_paths  [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_counts_path  , m_hists_emission_e_tankWater_counts_path   }, 
                                                                                        { m_hists_emission_mu_MRDsci_counts_path     , m_hists_emission_e_MRDsci_counts_path      } };
    const string       hists_emission_energies_names[ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_energies_name, m_hists_emission_e_tankWater_energies_name }, 
                                                                                        { m_hists_emission_mu_MRDsci_energies_name   , m_hists_emission_e_MRDsci_energies_name    } };
    const string       hists_emission_counts_names  [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_counts_name  , m_hists_emission_e_tankWater_counts_name   }, 
                                                                                        { m_hists_emission_mu_MRDsci_counts_name     , m_hists_emission_e_MRDsci_counts_name      } };
    const Energy       hists_emission_energy_mins   [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_energy_min   , m_hists_emission_e_tankWater_energy_min    }, 
                                                                                        { m_hists_emission_mu_MRDsci_energy_min      , m_hists_emission_e_MRDsci_energy_min       } };
    const Energy       hists_emission_energy_deltas [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_energy_delta , m_hists_emission_e_tankWater_energy_delta  }, 
                                                                                        { m_hists_emission_mu_MRDsci_energy_delta    , m_hists_emission_e_MRDsci_energy_delta     } };
    const unsigned int hists_emission_energy_nums   [ num_materials ][ num_particles ]{ { m_hists_emission_mu_tankWater_num          , m_hists_emission_e_tankWater_num           }, 
                                                                                        { m_hists_emission_mu_MRDsci_num             , m_hists_emission_e_MRDsci_num              } };
    const string       hists_transmission_paths        [ num_materials ]              {   m_hists_transmission_tankWater_path        , m_hists_transmission_MRDsci_path             };
    const string       hists_transmission_names        [ num_materials ]              {   m_hists_transmission_tankWater_name        , m_hists_transmission_MRDsci_name             };
    const Energy       hists_transmission_energy_mins  [ num_materials ]              {   m_hists_transmission_tankWater_energy_min  , m_hists_transmission_MRDsci_energy_min       };
    const Energy       hists_transmission_energy_deltas[ num_materials ]              {   m_hists_transmission_tankWater_energy_delta, m_hists_transmission_MRDsci_energy_delta     };
    const unsigned int hists_transmission_energy_nums  [ num_materials ]              {   m_hists_transmission_tankWater_num         , m_hists_transmission_MRDsci_num              };

    using DetectorResponsePredictorLoadEmissionPtr     = bool ( DetectorResponsePredictor::* )( const vector< string >&, const vector< string >&, 
                                                                                                const vector< string >&, const vector< string >&,
                                                                                                const vector< Energy >&                          );
    using DetectorResponsePredictorLoadtransmissionPtr = bool ( DetectorResponsePredictor::* )( const vector< string >&, const vector< string >&, 
                                                                                                const vector< Energy >&                          );

    DetectorResponsePredictorLoadEmissionPtr      load_hists_emissions  [ num_materials ]{ &DetectorResponsePredictor::load_hists_emission_tankWater    , 
                                                                                           &DetectorResponsePredictor::load_hists_emission_MRDsci        };
    DetectorResponsePredictorLoadtransmissionPtr load_hists_transmission[ num_materials ]{ &DetectorResponsePredictor::load_hists_transmission_tankWater, 
                                                                                           &DetectorResponsePredictor::load_hists_transmission_MRDsci    };

    for( unsigned int nParticle{ 0 }; nParticle < num_particles; nParticle++ ) {
        LogD( "Setting verbosity for DetectorResponsePredictor (nParticle=" + to_string( nParticle ) + ").", m_verbosity_debug );
        DetectorResponsePredictors[ nParticle ]->set_verbosity( m_verbosity_DetectorResponsePredictor );

        LogD( "Setting verbosity for THistReader (nParticle=" + to_string( nParticle ) + ").", m_verbosity_debug );
        DetectorResponsePredictors[ nParticle ]->set_verbosity_THistReader( m_verbosity_THistReader );

        LogD( "Setting particle for DetectorResponsePredictor (nParticle=" + to_string( nParticle ) + ").", m_verbosity_debug );
        DetectorResponsePredictors[ nParticle ]->set_particle_name( DetectorResponsePredictors_particles[ nParticle ] );
    }

    Energy           hists_emission_energy_cur        ;
    vector< string > hists_emission_energies_paths_cur;
    vector< string > hists_emission_counts_paths_cur  ;
    vector< string > hists_emission_energies_names_cur;
    vector< string > hists_emission_counts_names_cur  ;
    vector< Energy > hists_emission_energies_cur      ;
    Energy           hists_transmission_energy_cur   ;
    vector< string > hists_transmission_paths_cur    ;
    vector< string > hists_transmission_names_cur    ;
    vector< Energy > hists_transmission_energies_cur ;
    for( unsigned int nMaterial{ 0 }; nMaterial < num_materials; nMaterial++ ) {
        for( unsigned int nParticle{ 0 }; nParticle < num_particles; nParticle++ ) {
            hists_emission_energy_cur = hists_emission_energy_mins[ nMaterial ][ nParticle ];
            hists_emission_energies_paths_cur.clear();
            hists_emission_counts_paths_cur  .clear();
            hists_emission_energies_names_cur.clear();
            hists_emission_counts_names_cur  .clear();
            hists_emission_energies_cur      .clear();
            for( unsigned int nEnergy{ 0 }; nEnergy < hists_emission_energy_nums[ nMaterial ][ nParticle ]; nEnergy++ ) {
                hists_emission_energies_paths_cur.push_back( hists_emission_energies_paths[ nMaterial ][ nParticle ]                                );
                hists_emission_counts_paths_cur  .push_back( hists_emission_counts_paths  [ nMaterial ][ nParticle ]                                );
                hists_emission_energies_names_cur.push_back( hists_emission_energies_names[ nMaterial ][ nParticle ] + ";" + to_string( nEnergy+1 ) );
                hists_emission_counts_names_cur  .push_back( hists_emission_counts_names  [ nMaterial ][ nParticle ] + ";" + to_string( nEnergy+1 ) );
                hists_emission_energies_cur      .push_back( hists_emission_energy_cur                                                              );
                hists_emission_energy_cur += hists_emission_energy_deltas[ nMaterial ][ nParticle ];
            }
            LogD( "Loading emission histograms (Material=" + materialNames[ nMaterial ] + " and Particle=" + DetectorResponsePredictors_particles[ nParticle ] + ").", m_verbosity_debug );
            if( !( DetectorResponsePredictors[ nParticle ]->*load_hists_emissions[ nMaterial ] )( hists_emission_energies_paths_cur, hists_emission_counts_paths_cur,
                                                                                                  hists_emission_energies_names_cur, hists_emission_counts_names_cur,
                                                                                                  hists_emission_energies_cur                                        ) ) {
                string temp_string{ "Cannot load emission histograms (nMaterial=" };
                LogD( temp_string + to_string( nMaterial ) + " and nParticle=" + to_string( nParticle ) + ").", m_verbosity_error );
                LogD( "hists_emission_energies_paths_cur[0]: " + hists_emission_energies_paths_cur[ 0 ]  , m_verbosity_debug );
                LogD( "hists_emission_counts_paths_cur  [0]: " + hists_emission_counts_paths_cur  [ 0 ]  , m_verbosity_debug );
                LogD( "hists_emission_energies_names_cur[0]: " + hists_emission_energies_names_cur[ 0 ]  , m_verbosity_debug );
                LogD( "hists_emission_counts_names_cur  [0]: " + hists_emission_counts_names_cur  [ 0 ]  , m_verbosity_debug );
                LogD( "hists_emission_energies_cur      [0]: " + hists_emission_energies_cur      [ 0 ] ), m_verbosity_debug );
                return false;
            }

            if( nParticle != 0 ) {
                LogD( "Copying transmission histograms (Material=" + materialNames[ nMaterial ] + " and Particle=" + DetectorResponsePredictors_particles[ nParticle ] + ").", m_verbosity_debug );
                DetectorResponsePredictors[ nParticle ]->set_hist_transmission_tankWater( DetectorResponsePredictors[ 0 ]->get_hist_transmission_tankWater() );
                DetectorResponsePredictors[ nParticle ]->set_hist_transmission_MRDsci   ( DetectorResponsePredictors[ 0 ]->get_hist_transmission_MRDsci   () );
                if( !m_useSingleTH1forPhotonTransmission_tankWater )
                    DetectorResponsePredictors[ nParticle ]->set_hists_transmission_tankWater( DetectorResponsePredictors[ 0 ]->get_hists_transmission_tankWater() );
                if( !m_useSingleTH1forPhotonTransmission_MRDsci )
                    DetectorResponsePredictors[ nParticle ]->set_hists_transmission_MRDsci   ( DetectorResponsePredictors[ 0 ]->get_hists_transmission_MRDsci   () );
                continue;
            }
            hists_transmission_energy_cur = hists_transmission_energy_mins[ nMaterial ];
            hists_transmission_paths_cur   .clear();
            hists_transmission_names_cur   .clear();
            hists_transmission_energies_cur.clear();
            for( unsigned int nEnergy{ 0 }; nEnergy < hists_transmission_energy_nums[ nMaterial ]; nEnergy++ ) {
                hists_transmission_paths_cur   .push_back( hists_transmission_paths[ nMaterial ]                                );
                hists_transmission_names_cur   .push_back( hists_transmission_names[ nMaterial ] + ";" + to_string( nEnergy+1 ) );
                hists_transmission_energies_cur.push_back( hists_transmission_energy_cur                                        );
                hists_transmission_energy_cur += hists_transmission_energy_deltas[ nMaterial ];
            }
            LogD( "Loading transmission histograms (Material=" + materialNames[ nMaterial ] + ").", m_verbosity_debug );
            if( !( DetectorResponsePredictors[ nParticle ]->*load_hists_transmission[ nMaterial ] )( hists_transmission_paths_cur,
                                                                                                     hists_transmission_names_cur,
                                                                                                     hists_transmission_energies_cur ) ) {
                string temp_string{ "Cannot load transmission histograms (nMaterial=" };
                LogD( temp_string + to_string( nMaterial ) + ").", m_verbosity_error );
                LogD( "hists_transmission_paths_cur   [0]: " + hists_transmission_paths_cur   [ 0 ]  , m_verbosity_debug );
                LogD( "hists_transmission_names_cur   [0]: " + hists_transmission_names_cur   [ 0 ]  , m_verbosity_debug );
                LogD( "hists_transmission_energies_cur[0]: " + hists_transmission_energies_cur[ 0 ] ), m_verbosity_debug );
                return false;
            }
        }
    }

    const string hist_dEdX_tankWater_paths[ num_particles ]{ m_hists_dEdX_mu_tankWater_path, m_hists_dEdX_e_tankWater_path };
    const string hist_dEdX_tankSteel_paths[ num_particles ]{ m_hists_dEdX_mu_tankSteel_path, m_hists_dEdX_e_tankSteel_path };
    const string hist_dEdX_MRDsci_paths   [ num_particles ]{ m_hists_dEdX_mu_MRDsci_path   , m_hists_dEdX_e_MRDsci_path    };
    const string hist_dEdX_MRDiron_paths  [ num_particles ]{ m_hists_dEdX_mu_MRDiron_path  , m_hists_dEdX_e_MRDiron_path   };
    const string hist_dEdX_tankWater_names[ num_particles ]{ m_hists_dEdX_mu_tankWater_name, m_hists_dEdX_e_tankWater_name };
    const string hist_dEdX_tankSteel_names[ num_particles ]{ m_hists_dEdX_mu_tankSteel_name, m_hists_dEdX_e_tankSteel_name };
    const string hist_dEdX_MRDsci_names   [ num_particles ]{ m_hists_dEdX_mu_MRDsci_name   , m_hists_dEdX_e_MRDsci_name    };
    const string hist_dEdX_MRDiron_names  [ num_particles ]{ m_hists_dEdX_mu_MRDiron_name  , m_hists_dEdX_e_MRDiron_name   };
    for( unsigned int nParticle{ 0 }; nParticle < num_particles; nParticle++ ) {
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_tankWater( hist_dEdX_tankWater_paths[ nParticle ], hist_dEdX_tankWater_names[ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_tankSteel( hist_dEdX_tankSteel_paths[ nParticle ], hist_dEdX_tankSteel_names[ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_MRDsci   ( hist_dEdX_MRDsci_paths   [ nParticle ], hist_dEdX_MRDsci_names   [ nParticle ] );
        DetectorResponsePredictors[ nParticle ]->load_hist_dEdX_MRDiron  ( hist_dEdX_MRDiron_paths  [ nParticle ], hist_dEdX_MRDiron_names  [ nParticle ] );
    }

    // pause execution
    std::cin.get();
    return true;
}


bool ChargedLeptonLikelihoodReco::Execute()
{
    Log( "===========================================================================================", m_verbosity_debug, m_verbosity_ChargedLeptonLikelihoodReco );
    Log( "ChargedLeptonLikelihoodReco Tool: Executing", m_verbosity_debug, m_verbosity_ChargedLeptonLikelihoodReco );

    // Load digits
    if( !m_data->Stores.at( "RecoEvent" )->Get( "RecoDigit", m_digits ) ){
        string temp_string{ "Cannot retrieve RecoDigits. No digit from the RecoEvent!" };
        LogD( temp_string, m_verbosity_error ); 
        return false;
    }
    m_vtxGeo->LoadDigits( m_digits );

    return true;
}


bool ChargedLeptonLikelihoodReco::Finalise()
{
    delete m_DetectorResponsePredictor_mu;
    delete m_DetectorResponsePredictor_e ;

    return true;
}

inline void ChargedLeptonLikelihoodReco::Log_debug( const string& t_message, unsigned int t_verbosity, 
                                                    const string& t_file   , const string& t_function, int t_line ) {
    if( t_verbosity <= m_verbosity_ChargedLeptonLikelihoodReco )
        cout << "UserTool_ChargedLeptonLikelihoodReco || " << m_verbosity_map[ t_verbosity ] << " || " 
             << t_file << "::" << t_function << " (" << t_line << "): " << t_message << endl;
}

inline void ChargedLeptonLikelihoodReco::Log_debug( const string&& t_message, unsigned int t_verbosity,
                                                    const string& t_file   , const string& t_function, int t_line ) {
    if( t_verbosity <= m_verbosity_ChargedLeptonLikelihoodReco )
        cout << "UserTool_ChargedLeptonLikelihoodReco || " << m_verbosity_map[ t_verbosity ] << " || " 
             << t_file << "::" << t_function << " (" << t_line << "): " << t_message << endl;
}

inline bool ChargedLeptonLikelihoodReco::get_config_path( const string& t_variable_name, string& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load path variable `" };
        LogD( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    } else if( !exists( t_variable ) ) {
        string temp_string{ "Invalid path for `" };
        LogD( temp_string + t_variable_name + "`: \"" + t_variable + "\". Path does not exist.", m_verbosity_error );
        return false;
    } else if( !is_regular_file( t_variable ) ) {
        string temp_string{ "Invalid path for `" };
        LogD( temp_string + t_variable_name + "`: \"" + t_variable + "\". Path is not a \"regular file\" (cannot be a directory).", m_verbosity_error );
        return false;
    }

    LogD( "Loaded path variable `" + t_variable_name + "`: \"" + t_variable + "\".", m_verbosity_debug );
    return true;
}

inline bool ChargedLeptonLikelihoodReco::get_config_verbosity( const string& t_variable_name, unsigned int& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load verbosity variable `" };
        LogD( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    } else if( t_variable > 3 ) {
        string temp_string{ "Invalid verbosity value for `" };
        LogD( temp_string + t_variable_name + "`: \"" + to_string( t_variable ) + "\". Must be 3 or less (0=error, 1=warining, 2=message, and 3=debug).", m_verbosity_error );
        return false;
    }

    LogD( "Loaded verbosity variable `" + t_variable_name + "`: " + to_string( t_variable ) + ".", m_verbosity_debug );
    return true;
}

inline bool ChargedLeptonLikelihoodReco::get_config_histName( const string& t_variable_name, string& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load histogram name variable `" };
        LogD( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    } else if( t_variable.find( ';' ) != string::npos ) {
        string temp_string{ "Invalid histogram name for `" };
        LogD( temp_string + t_variable_name + "`: \"" + t_variable + "\". Must contain ';' (e.g. `histName;1`).", m_verbosity_error );
        return false;
    }

    LogD( "Loaded histogram name variable `" + t_variable_name + "`: \"" + t_variable + "\".", m_verbosity_debug );
    return true;
}

inline bool ChargedLeptonLikelihoodReco::get_config_unsignedInt( const string& t_variable_name, unsigned int& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load unsigned int `" };
        LogD( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    }

    LogD( "Loaded unsigned int `" + t_variable_name + "`: " + to_string( t_variable ) + ".", m_verbosity_debug );
    return true;
}

inline bool ChargedLeptonLikelihoodReco::get_config_double( const string& t_variable_name, double& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load double `" };
        LogD( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    }

    LogD( "Loaded double `" + t_variable_name + "`: " + to_string( t_variable ) + ".", m_verbosity_debug );
    return true;
}

inline bool ChargedLeptonLikelihoodReco::get_config_bool( const string& t_variable_name, bool& t_variable ) {
    if( !m_variables.Get( t_variable_name, t_variable ) ) {
        string temp_string{ "Unable to load bool `" };
        LogD( temp_string + t_variable_name + "`.", m_verbosity_error );
        return false;
    }

    LogD( "Loaded bool `" + t_variable_name + "`: " + to_string( t_variable ) + ".", m_verbosity_debug );
    return true;
}

template< typename MeasurementType >
inline bool ChargedLeptonLikelihoodReco::get_config_measurement( const string& t_variable_name_value, const string& t_variable_name_unit, MeasurementType& t_variable ) {
    double value;
    string unit;
    if( !m_variables.Get( t_variable_name_value, value ) ) {
        string temp_string{ "Unable to load measurement value `" };
        LogD( temp_string + t_variable_name_value + "`.", m_verbosity_error );
        return false;
    }
    if( !m_variables.Get( t_variable_name_unit, unit ) ) {
        string temp_string{ "Unable to load measurement unit `" };
        LogD( temp_string + t_variable_name_unit + "`.", m_verbosity_error );
        return false;
    }

    t_variable = MeasurementType( value, unit );
    LogD( "Loaded measurement `" + t_variable_name_value + "`: " + to_string( value ) + " " + unit + ".", m_verbosity_debug );
    return true;
}