//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                DetectorResponsePredictor.h                              //*//
//*/////////////////////////////////////////////////////////////////////////////////////////////*//
//*//                                                                                         //*//
//*// Author:                                                                                 //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                                           //*//
//*//   Created Febuary 13, 2023                                                              //*//
//*//                                                                                         //*//
//*// About:                                                                                  //*//
//*//   This file contains classes for generalized detector response prediction. All          //*//
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

#ifndef DETECTORRESPONSEPREDICTOR_H
#define DETECTORRESPONSEPREDICTOR_H

// STD
#include <vector>
#include <queue>
#include <string>
#include <cmath>

// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TVector2.h"

// DataModels
#include "VertexGeometry.h"
#include "THistReader.h"
#include "Particle.h"
#include "Detector.h"
#include "Measurements.h"

using std::vector;
using std::string;
    
// Index Structures
struct index_3{ unsigned int x, y, z; };
struct index_2{ unsigned int x, y   ; };

class DetectorResponsePredictor
{
public:
    /**////////////////////////
    /**/// (Con/De)structor ///
    /**////////////////////////
    /**/
    /**/ DetectorResponsePredictor() {}
    /**/~DetectorResponsePredictor() { reset_members(); }
    /**/
    /**////////////////////////
  
  
  
    /**/////////////////////////
    /**/// Get/Set Variables ///
    /**/////////////////////////
    /**/
    /**/ void         set_verbosity( unsigned int t_verbosity ) { m_verbosity = t_verbosity; }
    /**/ unsigned int get_verbosity() const                     { return m_verbosity;        }
    /**/
    /**/ void         set_verbosity_THistReader( unsigned int t_verbosity ) { m_verbosity_THistReader = t_verbosity; }
    /**/ unsigned int get_verbosity_THistReader() const                     { return m_verbosity_THistReader;        }
    /**/
    /**/ void         set_particle_name( const string& t_particle_name ) { m_particle_name = t_particle_name; }
    /**/ string       get_particle_name() const                          { return m_particle_name;            }
    /**/
    /**/ void reset_members();
    /**/
    /**/////////////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/// Load Histograms
    /**/ bool load_hists_emission_tankWater    ( const vector< string >& t_hists_energies_paths,
    /**/                                         const vector< string >& t_hists_counts_paths  ,
    /**/                                         const vector< string >& t_hists_energies_names,
    /**/                                         const vector< string >& t_hists_counts_names  ,
    /**/                                         const vector< Energy >& t_hists_energies       );
    /**/ bool load_hists_emission_MRDsci       ( const vector< string >& t_hists_energies_paths,
    /**/                                         const vector< string >& t_hists_counts_paths  ,
    /**/                                         const vector< string >& t_hists_energies_names,
    /**/                                         const vector< string >& t_hists_counts_names  ,
    /**/                                         const vector< Energy >& t_hists_energies       );
    /**/ 
    /**/ bool load_hists_transmission_tankWater( const vector< string >& t_hist_paths   ,
    /**/                                         const vector< string >& t_hist_names   ,
    /**/                                         const vector< Energy >& t_hist_energies );
    /**/ bool load_hists_transmission_MRDsci   ( const vector< string >& t_hist_paths   ,
    /**/                                         const vector< string >& t_hist_names   ,
    /**/                                         const vector< Energy >& t_hist_energies );
    /**/
    /**/ bool load_hist_dEdX_tankWater         ( const string& t_hist_path,
    /**/                                         const string& t_hist_name );
    /**/ bool load_hist_dEdX_tankSteel         ( const string& t_hist_path,
    /**/                                         const string& t_hist_name );
    /**/ bool load_hist_dEdX_MRDsci            ( const string& t_hist_path,
    /**/                                         const string& t_hist_name );
    /**/ bool load_hist_dEdX_MRDiron           ( const string& t_hist_path,
    /**/                                         const string& t_hist_name );
    /**/
    /**/// Get Histograms
    /**/ shared_ptr< THistMap< Energy, TH2D > > get_hists_emission_tankWater_energies() const { return m_hists_emission_tankWater_energies; }
    /**/ shared_ptr< THistMap< Energy, TH2D > > get_hists_emission_tankWater_counts  () const { return m_hists_emission_tankWater_counts;   }
    /**/ shared_ptr< THistMap< Energy, TH2D > > get_hists_emission_MRDsci_energies   () const { return m_hists_emission_MRDsci_energies;    }
    /**/ shared_ptr< THistMap< Energy, TH2D > > get_hists_emission_MRDsci_counts     () const { return m_hists_emission_MRDsci_counts;      }
    /**/
    /**/ shared_ptr< THistMap< Energy, TH1D > > get_hists_transmission_tankWater() const { return m_hists_transmission_tankWater; }
    /**/ shared_ptr< THistMap< Energy, TH1D > > get_hists_transmission_MRDsci   () const { return m_hists_transmission_MRDsci;    }
    /**/ shared_ptr<                   TH1D   > get_hist_transmission_tankWater () const { return m_hist_transmission_tankWater;  }
    /**/ shared_ptr<                   TH1D   > get_hist_transmission_MRDsci    () const { return m_hist_transmission_MRDsci;     }
    /**/
    /**/ shared_ptr<                   TH1D   > get_hist_dEdX_tankWater         () const { return m_hist_dEdX_tankWater; }
    /**/ shared_ptr<                   TH1D   > get_hist_dEdX_tankSteel         () const { return m_hist_dEdX_tankSteel; }
    /**/ shared_ptr<                   TH1D   > get_hist_dEdX_MRDsci            () const { return m_hist_dEdX_MRDsci;    }
    /**/ shared_ptr<                   TH1D   > get_hist_dEdX_MRDiron           () const { return m_hist_dEdX_MRDiron;   }
    /**/
    /**/// Set Histograms
    /**/ void set_hists_emission_tankWater_energies( shared_ptr< THistMap< Energy, TH2D > > t_hists_emission_tankWater_energies ) { m_hists_emission_tankWater_energies = t_hists_emission_tankWater_energies; }
    /**/ void set_hists_emission_tankWater_counts  ( shared_ptr< THistMap< Energy, TH2D > > t_hists_emission_tankWater_counts   ) { m_hists_emission_tankWater_counts   = t_hists_emission_tankWater_counts;   }
    /**/ void set_hists_emission_MRDsci_energies   ( shared_ptr< THistMap< Energy, TH2D > > t_hists_emission_MRDsci_energies    ) { m_hists_emission_MRDsci_energies    = t_hists_emission_MRDsci_energies;    }
    /**/ void set_hists_emission_MRDsci_counts     ( shared_ptr< THistMap< Energy, TH2D > > t_hists_emission_MRDsci_counts      ) { m_hists_emission_MRDsci_counts      = t_hists_emission_MRDsci_counts;      }
    /**/
    /**/ void set_hists_transmission_tankWater( shared_ptr< THistMap< Energy, TH1D > > t_hists_transmission_tankWater ) { m_hists_transmission_tankWater = t_hists_transmission_tankWater; }
    /**/ void set_hists_transmission_MRDsci   ( shared_ptr< THistMap< Energy, TH1D > > t_hists_transmission_MRDsci    ) { m_hists_transmission_MRDsci    = t_hists_transmission_MRDsci;    }
    /**/ void set_hist_transmission_tankWater ( shared_ptr          <         TH1D   > t_hist_transmission_tankWater  ) { m_hist_transmission_tankWater = t_hist_transmission_tankWater;   }
    /**/ void set_hist_transmission_MRDsci    ( shared_ptr          <         TH1D   > t_hist_transmission_MRDsci     ) { m_hist_transmission_MRDsci    = t_hist_transmission_MRDsci;      }
    /**/
    /**/ void set_hist_dEdX_tankWater( shared_ptr< TH1D > t_hist_dEdX_tankWater ) { m_hist_dEdX_tankWater = t_hist_dEdX_tankWater; }
    /**/ void set_hist_dEdX_tankSteel( shared_ptr< TH1D > t_hist_dEdX_tankSteel ) { m_hist_dEdX_tankSteel = t_hist_dEdX_tankSteel; }
    /**/ void set_hist_dEdX_MRDsci   ( shared_ptr< TH1D > t_hist_dEdX_MRDsci    ) { m_hist_dEdX_MRDsci    = t_hist_dEdX_MRDsci;    }
    /**/ void set_hist_dEdX_MRDiron  ( shared_ptr< TH1D > t_hist_dEdX_MRDiron   ) { m_hist_dEdX_MRDiron   = t_hist_dEdX_MRDiron;   }
    /**/
    /**/// Evaluate Histograms
    /**/ double eval_hists_emission_tankWater_energies( const double t_initialEnergy,
    /**/                                                const double t_trackLength  , 
    /**/                                                const double t_photonAngle   ) const;
    /**/ double eval_hists_emission_tankWater_counts  ( const double t_initialEnergy,
    /**/                                                const double t_trackLength  ,
    /**/                                                const double t_photonAngle   ) const;
    /**/ double eval_hists_emission_MRDsci_energies   ( const double t_initialEnergy,
    /**/                                                const double t_trackLength  , 
    /**/                                                const double t_photonAngle   ) const;
    /**/ double eval_hists_emission_MRDsci_counts     ( const double t_initialEnergy,
    /**/                                                const double t_trackLength  ,
    /**/                                                const double t_photonAngle   ) const;
    /**/
    /**/ double eval_hist_transmission_tankWater( const double t_photonEnergy  ) const;
    /**/ double eval_hist_transmission_MRDsci   ( const double t_photonEnergy  ) const;
    /**/ double eval_hist_dEdX_tankWater        ( const double t_primaryEnergy ) const;
    /**/ double eval_hist_dEdX_tankSteel        ( const double t_primaryEnergy ) const;
    /**/ double eval_hist_dEdX_MRDsci           ( const double t_primaryEnergy ) const;
    /**/ double eval_hist_dEdX_MRDiron          ( const double t_primaryEnergy ) const;
    /**/
    /**/// Misc.
    /**/ void   load_map_particleMasses( map< int, double >* t_map_particleMasses ) { m_map_particleMasses = t_map_particleMasses; }
    /**/ double eval_map_particleMasses( int                 t_PDG                ) { return m_map_particleMasses->at( t_PDG );    }
    /**/
    /**/ void   set_c( double t_c ) { m_c = t_c;  }
    /**/ double get_c()             { return m_c; }
    /**/
    /**////////////////////////////
  


    /**////////////////////////////////////
    /**/// Detector Response Prediction ///
    /**////////////////////////////////////
    /**/
    /**/ double get_expected_height( Particle* t_particle, Detector* t_detector, double t_time_start, double t_time_stop ) const;
    /**/ double get_expected_time  ( Particle* t_particle, Detector* t_detector ) const;
    /**/
    /**////////////////////////////////////
    


protected:
    /**//////////////
    /**/// Output ///
    /**//////////////
    /**/
    /**/ #define     LogD(                     t_message,              t_verbosity ) Log_debug( t_message, t_verbosity, __FILE__, __FUNCTION__, __LINE__ )
    /**/ inline void Log_debug( const string&  t_message, unsigned int t_verbosity, const string& t_file, const string& t_function, int t_line ) const;
    /**/ inline void Log_debug( const string&& t_message, unsigned int t_verbosity, const string& t_file, const string& t_function, int t_line ) const;
    /**/
    /**/ enum m_verbosity_enum{ m_verbosity_error   = 0,
    /**/                        m_verbosity_warning = 1,
    /**/                        m_verbosity_message = 2,
    /**/                        m_verbosity_debug   = 3 };
    /**/ map< unsigned int, string > m_verbosity_map{ { m_verbosity_error  , "ERROR"   },
    /**/                                              { m_verbosity_warning, "WARNING" },
    /**/                                              { m_verbosity_message, "MESSAGE" },
    /**/                                              { m_verbosity_debug  , "DEBUG"   } };
    /**/
    /**//////////////


    
    /**////////////////
    /**/// Settings ///
    /**////////////////
    /**/
    /**/ unsigned int m_verbosity            { m_verbosity_warning };
    /**/ unsigned int m_verbosity_THistReader{ m_verbosity_warning };
    /**/ double       m_PMTdiameter          { 8 * 0.0254          }; // 8 inches in meters
    /**/ double       m_LAPPDsideLength      { 20 * 0.01           }; // 20 cm in meters
    /**/ string       m_particle_name        { ""                  };
    /**/
    /**////////////////



    /**////////////////////////////
    /**/// Physical Information ///
    /**////////////////////////////
    /**/
    /**/// Emission Histograms
    /**/ vector    < Energy                   > m_hists_emission_initialEnergies;
    /**/ shared_ptr< THistMap< Energy, TH2D > > m_hists_emission_tankWater_energies{ nullptr };
    /**/ shared_ptr< THistMap< Energy, TH2D > > m_hists_emission_tankWater_counts  { nullptr };
    /**/ shared_ptr< THistMap< Energy, TH2D > > m_hists_emission_MRDsci_energies   { nullptr };
    /**/ shared_ptr< THistMap< Energy, TH2D > > m_hists_emission_MRDsci_counts     { nullptr };
    /**/ double                                 m_binWidth_s_tankWater;
    /**/ double                                 m_binWidth_s_MRDsci;
    /**/ double                                 m_binWidth_theta_tankWater;
    /**/ double                                 m_binWidth_theta_MRDsci;
    /**/ double                                 m_binWidth_phi_tankWater;
    /**/ double                                 m_binWidth_phi_MRDsci;
    /**/
    /**/// Transmission Histograms
    /**/ shared_ptr< THistMap< Energy, TH1D > > m_hists_transmission_tankWater{ nullptr };
    /**/ shared_ptr< THistMap< Energy, TH1D > > m_hists_transmission_MRDsci   { nullptr };
    /**/ shared_ptr<                   TH1D   > m_hist_transmission_tankWater { nullptr };
    /**/ shared_ptr<                   TH1D   > m_hist_transmission_MRDsci    { nullptr };
    /**/
    /**/// Stopping Power Histograms
    /**/ shared_ptr< TH1D > m_hist_dEdX_tankWater{ nullptr };
    /**/ shared_ptr< TH1D > m_hist_dEdX_tankSteel{ nullptr };
    /**/ shared_ptr< TH1D > m_hist_dEdX_MRDsci   { nullptr };
    /**/ shared_ptr< TH1D > m_hist_dEdX_MRDiron  { nullptr };
    /**/
    /**/// Misc.
    /**/ map< int, double >* m_map_particleMasses{ new map< int, double >{ { 11, 0.5109989461 }, { -11, 0.5109989461 }, 
    /**/                                                                   { 13, 105.6583755  }, { -13, 105.6583755  } } };
    /**/ double              m_c{ 299792458 };
    /**/
    /**////////////////////////////



    /**////////////////////////////
    /**/// Load/Eval Histograms ///
    /**////////////////////////////
    /**/
    /**/// Check Histogram Ax(is/es)
    /**/ bool check_axis( const TAxis* t_axis ) const;
    /**/ bool check_axes( const vector< TAxis* >& t_axes, double t_min = 0, double t_max = 0, int t_nBins = 0, double t_epsilon = 1e-6 ) const;
    /**/
    /**/// Load Histograms
    /**/ bool load_hists_emission    (       shared_ptr< THistMap< Energy, TH2D > >& t_hists_energies       ,
    /**/                                     shared_ptr< THistMap< Energy, TH2D > >& t_hists_counts         ,
    /**/                               const vector    < string                   >& t_hists_energies_paths ,
    /**/                               const vector    < string                   >& t_hists_counts_paths   ,
    /**/                               const vector    < string                   >& t_hists_energies_names ,
    /**/                               const vector    < string                   >& t_hists_counts_names   ,
    /**/                               const vector    < Energy                   >& t_hists_initialEnergies,
    /**/                                     double                                & t_binWidth_s           ,
    /**/                                     double                                & t_binWidth_theta       ,
    /**/                                     double                                & t_binWidth_phi         ,
    /**/                               const string                                & t_hists_tag             );
    /**/ bool load_hists_transmission(       shared_ptr< THistMap< Energy, TH1D > >& t_hist_transmission    ,
    /**/                               const string                                & t_hist_path            ,
    /**/                               const string                                & t_hist_name             );
    /**/
    /**/ template< typename type_ID, typename type_hist >
    /**/ bool load_hists            (       shared_ptr< THistMap< type_ID, type_hist > >& t_hists         ,
    /**/                              const vector              < string               >& t_hists_paths   ,
    /**/                              const vector              < string               >& t_hists_names   ,
    /**/                              const vector              < type_ID              >& t_hists_IDs     );
    /**/ template< typename type_hist >
    /**/ bool load_hist             (       shared_ptr< type_hist >& t_hist         ,
    /**/                              const string                 & t_hist_path    ,
    /**/                              const string                 & t_hist_name    ,
    /**/                              const string                 & t_hist_particle );
    /**/
    /**/// Evaluate Histograms
    /**/ pair< int, int > get_closestEmissionHists     ( const shared_ptr< THistMap< Energy, TH2D > >  t_hists_emission,
    /**/                                                 const double                                  t_initialEnergy    ) const;
    /**/ double           eval_hists_emission_values   ( const shared_ptr< THistMap< Energy, TH2D > >  t_hists_emission,
    /**/                                                 const double                                  t_initialEnergy   ,
    /**/                                                 const double                                  t_trackLength     ,
    /**/                                                 const double                                  t_photonAngle      ) const;
    /**/ double           eval_hists_emission_indicies ( const shared_ptr< THistMap< Energy, TH2D > >  t_hists_emission,
    /**/                                                 const double                                  t_initialEnergy   ,
    /**/                                                 const unsigned int                            t_trackLengthIndex,
    /**/                                                 const unsigned int                            t_photonAngleIndex ) const;
    /**/ double           eval_hist_value              ( const shared_ptr< TH1D >                    & t_hist            ,
    /**/                                                 const double                                  t_x                ) const;
    /**/ double           eval_hist_index              ( const shared_ptr< TH1D >                    & t_hist            ,
    /**/                                                 const unsigned int                            t_xIndex           ) const;
    /**/
    /**/// Misc.
    /**/ template< typename type_hist >
    /**/ bool make_averageTH1( shared_ptr< THistMap< Energy, type_hist > > t_hists, shared_ptr< type_hist >& t_hist, TString t_name = "", TString t_title = "" );
    /**/
    /**////////////////////////////
    
    
    
    /**//////////////////////////////
    /**/// Helpful TVector Things ///
    /**//////////////////////////////
    /**/
    /**/ double get_angle   ( const TVector3& t_vector_1, const TVector3& t_vector_2 ) const;
    /**/ double get_distance( const TVector3& t_point_1 , const TVector3& t_point_2  ) const;
    /**/
    /**/ void normalize( TVector3& t_vector ) const;
    /**/
    /**/ const TVector3 m_vector_x{ 1, 0, 0 };
    /**/ const TVector3 m_vector_y{ 0, 1, 0 };
    /**/ const TVector3 m_vector_z{ 0, 0, 1 };
    /**/
    /**//////////////////////////////
    
    
    
    /**////////////////////////////////////
    /**/// Emission Hists Bin Functions ///
    /**////////////////////////////////////
    /**/
    /**/ int get_bin_index( const double t_value, const double t_min  , 
    /**/                    const double t_max  , const int    t_nBins ) const;
    /**/ int get_bin_index( const double t_value, const TAxis* t_axis  ) const;
    /**/
    /**/ index_3            get_bin_index_detector_center  ( const TVector3& t_particle_position_init, const TVector3& t_particle_direction, 
    /**/                                                     const TVector3& t_detector_position     , const TH2D    * t_hist_emission     ,
    /**/                                                     const double    t_particle_trackLength                                    ) const;
    /**/ vector< index_3 >* get_bin_indicies_detector_total( const TH2D    * t_hist_emission    , bool ( *t_get_isDetector )( const index_3 ),
    /**/                                                     const index_3   t_index_start      , const index_3 t_index_max            ) const;
    /**/
    /**/ double get_bin_value( const unsigned int t_index, const double  t_min  , 
    /**/                       const double       t_max  , const int     t_nBins ) const;
    /**/ double get_bin_value( const unsigned int t_index, const TAxis*  t_axis  ) const;
    /**/
    /**////////////////////////////////////


    
    /**/////////////////////////////
    /**/// Is Detector Functions ///
    /**/////////////////////////////
    /**/
    /**/ bool get_isDetector_tankPMT( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
    /**/                              const TVector3& t_detector_position     , const TVector3& t_detector_direction,
    /**/                              const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const;
    /**/ bool get_isDetector_LAPPD  ( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
    /**/                              const TVector3& t_detector_position     , const TVector3& t_detector_direction,
    /**/                              const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const;
    /**/ bool get_isDetector_MRD    ( const TVector3& t_particle_position_init, const TVector3& t_particle_direction,
    /**/                              const TVector3& t_detector_position     , const TVector3& t_detector_direction,
    /**/                              const TH2D    * t_hist_emission         , const index_3   t_binIndex           ) const;
    /**/
    /**/////////////////////////////



    /**////////////////////////////
    /**/// Acceptance Functions ///
    /**////////////////////////////
    /**/
    /**/ double get_acceptance_tankPMT( const double t_incidenceAngle, const int t_detectorID ) const;
    /**/ double get_acceptance_LAPPD  ( const double t_incidenceAngle, const int t_detectorID ) const;
    /**/ double get_acceptance_MRD    ( const double t_incidenceAngle, const int t_detectorID ) const;
    /**/
    /**////////////////////////////



    /**//////////////////////////////
    /**/// transmission Functions ///
    /**//////////////////////////////
    /**/
    /**/ double get_transmission_tankWater( const double t_distance, const double t_photonEnergy ) const;
    /**/ double get_transmission_MRDsci   ( const double t_distance, const double t_photonEnergy ) const;
    /**/
    /**///////////////////////////////
    
    
    
    /**//////////////////////////
    /**/// Physical Functions ///
    /**//////////////////////////
    /**/
    /**/ double       get_particle_mass         ( Particle* t_particle                ) const;
    /**/ double       get_particlePosition_value( Particle* t_particle, double t_time ) const;
    /**/ unsigned int get_particlePosition_index( Particle* t_particle, double t_time ) const;
    /**/
    /**//////////////////////////
};

#endif
