#ifndef ChargedLeptonLikelihoodReco_H
#define ChargedLeptonLikelihoodReco_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TMinuit.h"

#include "VertexGeometry.h"
#include "DetectorResponsePredictor.h"

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
    /**/ unsigned int m_verbosity_ChargedLeptonLikelihoodReco ;
    /**/ unsigned int m_verbosity_DetectorResponsePredictor   ;
    /**/ unsigned int m_verbosity_THistReader                 ;
    /**/
    /**/ string       m_hists_emission_mu_tankWater_energies_path        ;
    /**/ string       m_hists_emission_e_tankWater_energies_path         ;
    /**/ string       m_hists_emission_mu_MRDsci_energies_path           ;
    /**/ string       m_hists_emission_e_MRDsci_energies_path            ;
    /**/
    /**/ string       m_hists_emission_mu_tankWater_counts_path        ;
    /**/ string       m_hists_emission_e_tankWater_counts_path         ;
    /**/ string       m_hists_emission_mu_MRDsci_counts_path           ;
    /**/ string       m_hists_emission_e_MRDsci_counts_path            ;
    /**/
    /**/ string       m_hists_emission_mu_tankWater_energies_name        ;
    /**/ string       m_hists_emission_e_tankWater_energies_name         ;
    /**/ string       m_hists_emission_mu_MRDsci_energies_name           ;
    /**/ string       m_hists_emission_e_MRDsci_energies_name            ;
    /**/
    /**/ string       m_hists_emission_mu_tankWater_counts_name        ;
    /**/ string       m_hists_emission_e_tankWater_counts_name         ;
    /**/ string       m_hists_emission_mu_MRDsci_counts_name           ;
    /**/ string       m_hists_emission_e_MRDsci_counts_name            ;
    /**/
    /**/ unsigned int m_hists_emission_mu_tankWater_energy_min  ;   
    /**/ unsigned int m_hists_emission_e_tankWater_energy_min   ;   
    /**/ unsigned int m_hists_emission_mu_MRDsci_energy_min     ;   
    /**/ unsigned int m_hists_emission_e_MRDsci_energy_min      ;   
    /**/
    /**/ unsigned int m_hists_emission_mu_tankWater_energy_delta;
    /**/ unsigned int m_hists_emission_e_tankWater_energy_delta ;
    /**/ unsigned int m_hists_emission_mu_MRDsci_energy_delta   ;
    /**/ unsigned int m_hists_emission_e_MRDsci_energy_delta    ;
    /**/
    /**/ unsigned int m_hists_emission_mu_tankWater_num         ;
    /**/ unsigned int m_hists_emission_e_tankWater_num          ;
    /**/ unsigned int m_hists_emission_mu_MRDsci_num            ;
    /**/ unsigned int m_hists_emission_e_MRDsci_num             ;
    /**/
    /**/ string       m_hists_dEdX_mu_tankWater_path            ;
    /**/ string       m_hists_dEdX_e_tankWater_path             ;
    /**/ string       m_hists_dEdX_mu_tankSteel_path            ;
    /**/ string       m_hists_dEdX_e_tankSteel_path             ;
    /**/ string       m_hists_dEdX_mu_MRDsci_path               ;
    /**/ string       m_hists_dEdX_e_MRDsci_path                ;
    /**/ string       m_hists_dEdX_mu_MRDiron_path              ;
    /**/ string       m_hists_dEdX_e_MRDiron_path               ;
    /**/
    /**/ string       m_hists_dEdX_mu_tankWater_name            ;
    /**/ string       m_hists_dEdX_e_tankWater_name             ;
    /**/ string       m_hists_dEdX_mu_tankSteel_name            ;
    /**/ string       m_hists_dEdX_e_tankSteel_name             ;
    /**/ string       m_hists_dEdX_mu_MRDsci_name               ;
    /**/ string       m_hists_dEdX_e_MRDsci_name                ;
    /**/ string       m_hists_dEdX_mu_MRDiron_name              ;
    /**/ string       m_hists_dEdX_e_MRDiron_name               ;
    /**/
    /**/ string       m_hists_transmittance_mu_tankWater_path   ;
    /**/ string       m_hists_transmittance_e_tankWater_path    ;
    /**/ string       m_hists_transmittance_mu_MRDsci_path      ;
    /**/ string       m_hists_transmittance_e_MRDsci_path       ;
    /**/
    /**/ string       m_hists_transmittance_mu_tankWater_name   ;
    /**/ string       m_hists_transmittance_e_tankWater_name    ;
    /**/ string       m_hists_transmittance_mu_MRDsci_name      ;
    /**/ string       m_hists_transmittance_e_MRDsci_name       ;
    /**/
    /**/ inline bool get_config_path       ( const string& t_variable_name, string      & t_variable );
    /**/ inline bool get_config_verbosity  ( const string& t_variable_name, unsigned int& t_variable );
    /**/ inline bool get_config_histName   ( const string& t_variable_name, string      & t_variable );
    /**/ inline bool get_config_unsignedInt( const string& t_variable_name, unsigned int& t_variable );
    /**/ inline bool get_config_double     ( const string& t_variable_name, double      & t_variable );
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
