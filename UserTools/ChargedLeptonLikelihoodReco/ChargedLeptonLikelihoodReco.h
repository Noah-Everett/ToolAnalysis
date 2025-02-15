#ifndef ChargedLeptonLikelihoodReco_H
#define ChargedLeptonLikelihoodReco_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TMinuit.h"

#include "VertexGeometry.h"
#include "DetectorResponsePredictor.h"
#include "Measurements.h"

using std::string;
using std::vector;

/**
 * * \class ChargedLeptonLikelihoodReco
 *  *
 *   * Does stuff... Ill change this later... but will probably forget ):
 *   *
 *   * $Author: Noah Everett $
 *   * $Date: 2022/09/28 00:00:00 $
 *   * Contact: noah.everett@mines.sdsmt.edu
 *   */

class ChargedLeptonLikelihoodReco: public Tool
{
public:
    ChargedLeptonLikelihoodReco() {}
    bool Initialise( string configfile, DataModel& data );
    bool Execute   ();
    bool Finalise  ();

    void make_heatMap();

    bool load_hists( string t_hists_dir, void ( *loadingFunction )( vector< string >, vector< int >, vector< string > ) );

    double find_eventParams( Particle t_particle );

private:
    /**/////////////////
    /**/// Messaging ///
    /**/////////////////
    /**/
    /**/ #define     LogD(                        t_message      ,                  t_verbosity ) Log_debug( t_message, t_verbosity, __FILE__, __FUNCTION__, __LINE__ )
    /**/ inline void Log_debug   ( const string&  t_message      , unsigned int     t_verbosity, const string& t_file, const string& t_function, int t_line );
    /**/ inline void Log_debug   ( const string&& t_message      , unsigned int     t_verbosity, const string& t_file, const string& t_function, int t_line );
    /**/ inline void print_config( const string&  t_variable_name,          string& t_variable  );
    /**/ enum m_verbosity_enum{ m_verbosity_error   = 0,
    /**/                        m_verbosity_warning = 1,
    /**/                        m_verbosity_message = 2,
    /**/                        m_verbosity_debug   = 3 };
    /**/ map< unsigned int, string > m_verbosity_map{ { m_verbosity_error  , "ERROR"   },
    /**/                                              { m_verbosity_warning, "WARNING" },
    /**/                                              { m_verbosity_message, "MESSAGE" },
    /**/                                              { m_verbosity_debug  , "DEBUG"   } };
    /**/
    /**/////////////////



    /**///////////////////////////////
    /**/// Configuration Variables ///
    /**///////////////////////////////
    /**/
    /**////////////// Verbosities
    /**/ unsigned int m_verbosity_ChargedLeptonLikelihoodReco      ;
    /**/ unsigned int m_verbosity_DetectorResponsePredictor        ;
    /**/ unsigned int m_verbosity_THistReader                      ;
    /**/
    /**////////////// Emission Histograms
    /**/// Paths
    /**/ string       m_hists_emission_mu_tankWater_energies_path  ;
    /**/ string       m_hists_emission_e_tankWater_energies_path   ;
    /**/ string       m_hists_emission_mu_MRDsci_energies_path     ;
    /**/ string       m_hists_emission_e_MRDsci_energies_path      ;
    /**/ string       m_hists_emission_mu_tankWater_counts_path    ;
    /**/ string       m_hists_emission_e_tankWater_counts_path     ;
    /**/ string       m_hists_emission_mu_MRDsci_counts_path       ;
    /**/ string       m_hists_emission_e_MRDsci_counts_path        ;
    /**/
    /**/// Names
    /**/ string       m_hists_emission_mu_tankWater_energies_name  ;
    /**/ string       m_hists_emission_e_tankWater_energies_name   ;
    /**/ string       m_hists_emission_mu_MRDsci_energies_name     ;
    /**/ string       m_hists_emission_e_MRDsci_energies_name      ;
    /**/ string       m_hists_emission_mu_tankWater_counts_name    ;
    /**/ string       m_hists_emission_e_tankWater_counts_name     ;
    /**/ string       m_hists_emission_mu_MRDsci_counts_name       ;
    /**/ string       m_hists_emission_e_MRDsci_counts_name        ;
    /**/
    /**/// Starting Energies
    /**/ Energy       m_hists_emission_mu_tankWater_energy_min     ;   
    /**/ Energy       m_hists_emission_e_tankWater_energy_min      ;   
    /**/ Energy       m_hists_emission_mu_MRDsci_energy_min        ;   
    /**/ Energy       m_hists_emission_e_MRDsci_energy_min         ;   
    /**/
    /**/// Energy Deltas
    /**/ Energy       m_hists_emission_mu_tankWater_energy_delta   ;
    /**/ Energy       m_hists_emission_e_tankWater_energy_delta    ;
    /**/ Energy       m_hists_emission_mu_MRDsci_energy_delta      ;
    /**/ Energy       m_hists_emission_e_MRDsci_energy_delta       ;
    /**/
    /**/// Number of Energies
    /**/ unsigned int m_hists_emission_mu_tankWater_num            ;
    /**/ unsigned int m_hists_emission_e_tankWater_num             ;
    /**/ unsigned int m_hists_emission_mu_MRDsci_num               ;
    /**/ unsigned int m_hists_emission_e_MRDsci_num                ;
    /**/
    /**////////////// dEdX Histograms
    /**/// Paths
    /**/ string       m_hists_dEdX_mu_tankWater_path               ;
    /**/ string       m_hists_dEdX_e_tankWater_path                ;
    /**/ string       m_hists_dEdX_mu_tankSteel_path               ;
    /**/ string       m_hists_dEdX_e_tankSteel_path                ;
    /**/ string       m_hists_dEdX_mu_MRDsci_path                  ;
    /**/ string       m_hists_dEdX_e_MRDsci_path                   ;
    /**/ string       m_hists_dEdX_mu_MRDiron_path                 ;
    /**/ string       m_hists_dEdX_e_MRDiron_path                  ;
    /**/
    /**/// Names
    /**/ string       m_hists_dEdX_mu_tankWater_name               ;
    /**/ string       m_hists_dEdX_e_tankWater_name                ;
    /**/ string       m_hists_dEdX_mu_tankSteel_name               ;
    /**/ string       m_hists_dEdX_e_tankSteel_name                ;
    /**/ string       m_hists_dEdX_mu_MRDsci_name                  ;
    /**/ string       m_hists_dEdX_e_MRDsci_name                   ;
    /**/ string       m_hists_dEdX_mu_MRDiron_name                 ;
    /**/ string       m_hists_dEdX_e_MRDiron_name                  ;
    /**/
    /**////////////// Transmission Histograms
    /**/// Use Single Histogram
    /**/ bool         m_useSingleTH1forPhotonTransmission_tankWater;
    /**/ bool         m_useSingleTH1forPhotonTransmission_MRDsci   ;
    /**/
    /**/// Paths
    /**/ string       m_hists_transmission_tankWater_path          ;
    /**/ string       m_hists_transmission_MRDsci_path             ;
    /**/ string       m_hists_transmission_tankWater_name          ;
    /**/ string       m_hists_transmission_MRDsci_name             ;
    /**/
    /**/// Starting Energies
    /**/ Energy       m_hists_transmission_tankWater_energy_min    ;
    /**/ Energy       m_hists_transmission_MRDsci_energy_min       ;
    /**/
    /**/// Energy Deltas
    /**/ Energy       m_hists_transmission_tankWater_energy_delta  ;
    /**/ Energy       m_hists_transmission_MRDsci_energy_delta     ;
    /**/
    /**/// Number of Energies
    /**/ unsigned int m_hists_transmission_tankWater_num           ;
    /**/ unsigned int m_hists_transmission_MRDsci_num              ;
    /**/
    /**///////////// Load Configuration Variables
    /**/ inline bool get_config_path       ( const string& t_variable_name      , string         & t_variable );
    /**/ inline bool get_config_verbosity  ( const string& t_variable_name      , unsigned int   & t_variable );
    /**/ inline bool get_config_histName   ( const string& t_variable_name      , string         & t_variable );
    /**/ inline bool get_config_unsignedInt( const string& t_variable_name      , unsigned int   & t_variable );
    /**/ inline bool get_config_double     ( const string& t_variable_name      , double         & t_variable );
    /**/ inline bool get_config_bool       ( const string& t_variable_name      , bool           & t_variable );
    /**/ template< typename MeasurementType >
    /**/ inline bool get_config_measurement( const string& t_variable_name_value, 
    /**/                                     const string& t_variable_name_unit , MeasurementType& t_variable );
    /**/
    /**///////////////////////////////



    /**//////////////////
    /**/// DataModels ///
    /**//////////////////
    /**/
    /**/ VertexGeometry           * m_vtxGeo{ VertexGeometry::Instance() };
    /**/ vector< RecoDigit >      * m_digits{ nullptr };
    /**/ DetectorResponsePredictor* m_DetectorResponsePredictor_mu{ new DetectorResponsePredictor() };
    /**/ DetectorResponsePredictor* m_DetectorResponsePredictor_e { new DetectorResponsePredictor() };
    /**/
    /**//////////////////

    double m_likelyhood;

    Particle* m_particle_cur { nullptr };
    Particle* m_particle_best{ nullptr };

    TMinuit* m_minuit{ nullptr };


};



#endif
