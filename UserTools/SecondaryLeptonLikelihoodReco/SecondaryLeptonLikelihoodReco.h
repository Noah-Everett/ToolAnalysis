#ifndef SecondaryLeptonLikelihoodReco_H
#define SecondaryLeptonLikelihoodReco_H

#include <string>
#include <iostream>

#include "Tool.h"
#include "TMinuit.h"

#include "VertexGeometry.h"
#include "DetectorResponsePredictor.h"

using std::string;
using std::vector;

/**
 * * \class SecondaryLeptonLikelihoodReco
 *  *
 *   * Does stuff... Ill change this later... but will probably forget ):
 *   *
 *   * $Author: Noah Everett $
 *   * $Date: 2022/09/28 00:00:00 $
 *   * Contact: noah.everett@mines.sdsmt.edu
 *   */

class SecondaryLeptonLikelihoodReco: public Tool
{
public:
    SecondaryLeptonLikelihoodReco() {}
    bool Initialise( string configfile, DataModel& data );
    bool Execute   ();
    bool Finalise  ();

    void make_heatMap();

    bool load_hists( string t_hists_dir, void ( *loadingFunction )( vector< string >, vector< int >, vector< string > ) );

    double find_eventParams( Particle t_particle );

private:
    /**//////////////////////
    /**/// User Variables ///
    /**//////////////////////
    /**/
    /**/enum m_verbosity_enum { m_verbosity_error   = 0,
    /**/                        m_verbosity_warning = 1,
    /**/                        m_verbosity_message = 2,
    /**/                        m_verbosity_debug   = 3 };
    /**/
    /**//////////////////////



    /**///////////////////////////////
    /**/// Configuration Variables ///
    /**///////////////////////////////
    /**/
    /**/ unsigned int    m_verbosity_SecondaryLeptonLikelihoodReco;
    /**/ unsigned int    m_verbosity_DetectorResponsePRedictor    ;
    /**/
    /**/ string          m_hists_emission_mu_water_path           ;
    /**/ string          m_hists_emission_e_water_path            ;
    /**/ string          m_hists_emission_mu_sci_path             ;
    /**/ string          m_hists_emission_e_sci_path              ;
    /**/
    /**/ unsigned int    m_hists_emission_mu_water_energy_min     ;   
    /**/ unsigned int    m_hists_emission_e_water_energy_min      ;   
    /**/ unsigned int    m_hists_emission_mu_sci_energy_min       ;   
    /**/ unsigned int    m_hists_emission_e_sci_energy_min        ;   
    /**/
    /**/ unsigned double m_hists_emission_mu_water_energy_delta   ;
    /**/ unsigned double m_hists_emission_e_water_energy_delta    ;
    /**/ unsigned double m_hists_emission_mu_sci_energy_delta     ;
    /**/ unsigned double m_hists_emission_e_sci_energy_delta      ;
    /**/
    /**/ unsigned int    m_hists_emission_mu_water_num            ;
    /**/ unsigned int    m_hists_emission_e_water_num             ;
    /**/ unsigned int    m_hists_emission_mu_sci_num              ;
    /**/ unsigned int    m_hists_emission_e_sci_num               ;
    /**/
    /**/ string          m_hists_emission_mu_water_name           ;
    /**/ string          m_hists_emission_e_water_name            ;
    /**/ string          m_hists_emission_mu_sci_name             ;
    /**/ string          m_hists_emission_e_sci_name              ;
    /**/
    /**/ string          m_hists_dEdX_mu_water_path               ;
    /**/ string          m_hists_dEdX_e_water_path                ;
    /**/ string          m_hists_dEdX_mu_sci_path                 ;
    /**/ string          m_hists_dEdX_e_sci_path                  ;
    /**/
    /**/ string          m_hists_dEdX_mu_water_path               ;
    /**/ string          m_hists_dEdX_e_water_path                ;
    /**/ string          m_hists_dEdX_mu_sci_path                 ;
    /**/ string          m_hists_dEdX_e_sci_path                  ;
    /**/
    /**/ string          m_hists_dEdX_mu_water_name               ;
    /**/ string          m_hists_dEdX_e_water_name                ;
    /**/ string          m_hists_dEdX_mu_sci_name                 ;
    /**/ string          m_hists_dEdX_e_sci_name                  ;
    /**/
    /**///////////////////////////////


    VertexGeometry     * m_vtxGeo{ VertexGeometry::Instance() };
    vector< RecoDigit >* m_digits{ nullptr };

    DetectorResponsePredictor* m_detectorReponsePredictor
    double m_likelyhood;

    Particle* m_particle_cur { nullptr };
    Particle* m_particle_best{ nullptr };

    TMinuit* m_minuit{ nullptr };
};



#endif
