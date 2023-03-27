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
    // User variables
    int m_verbosity{ 1 };
    enum m_verbosity_enum { m_verbosity_error   = 0,
                            m_verbosity_warning = 1,
                            m_verbosity_message = 2,
                            m_verbosity_debug   = 3 };

  string m_hists_emission_mu_water_dir;
  string m_hists_emission_e_water_dir;
  string m_hists_emission_mu_sci_dir;
  string m_hists_emission_e_sci_dir;

  VertexGeometry     * m_vtxGeo{ VertexGeometry::Instance() };
  vector< RecoDigit >* m_digits{ nullptr };

  DetectorResponsePredictor* m_detectorReponsePredictor
  double m_likelyhood;

  Particle* m_particle_cur { nullptr };
  Particle* m_particle_best{ nullptr };

  TMinuit* m_minuit{ nullptr };
};



#endif
