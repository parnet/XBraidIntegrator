//
// Created by parnet on 09.06.21.
//

//  zzzzz
// Created by parnet on 12.05.19.
//


#include "bridge/util.h"
#include "bridge/util_domain_algebra_dependent.h"

#include "limex_integrator_factory.h"
#include "simple_integrator_factory.h"
#include "discontinuity_integrator_factory.h"// chk

#include "time_integrator_linear_adaptive_factory.h"// chk
#include "const_step_linear_time_integrator_factory.h"// chk
#include "linear_time_integrator_factory.h"



using namespace std;
using namespace ug::bridge;

namespace ug {

    namespace XBraidIntegrator {

        struct Functionality {


            template<typename TDomain, typename TAlgebra>
            static void DomainAlgebra(Registry &reg, string grp) {

                string suffix = GetDomainAlgebraSuffix<TDomain, TAlgebra>();
                string tag = GetDomainAlgebraTag<TDomain, TAlgebra>();




                // Integrator Factory
                {
                    //LimexFactory (registered by XBraidForUG4)
                    {
                        //typedef IntegratorFactory<TDomain,TAlgebra> TIntegratorFactory;
                        //typedef IntegratorFactory<TDomain,TAlgebra> TIntegratorFactory;
                        //string name_gf = string("IntegratorFactory").append(suffix);
                        //reg.add_class_<TIntegratorFactory>(name_gf, grp); // todo constructor set by executor
                        //reg.add_class_to_group(name_gf, "IntegratorFactory", tag);
                    }

                    {
                        typedef IntegratorFactory<TDomain,TAlgebra> TIntegratorFactory;
                        typedef LimexFactory<TDomain, TAlgebra> TLimexFactory;
                        string name_gf = string("LimexFactory").append(suffix);
                        reg.add_class_<TLimexFactory,TIntegratorFactory>(name_gf, grp) // todo constructor set by executor
                                .add_constructor()
                                .add_method("set_domain_disc", &TLimexFactory::set_domain_disc, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_solver", &TLimexFactory::set_solver, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_dt_max", &TLimexFactory::set_dt_max, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_dt_min", &TLimexFactory::set_dt_min, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_tol", &TLimexFactory::set_tol, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_level_factor", &TLimexFactory::set_level_factor, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_error_estimator", &TLimexFactory::set_error_estimator, "None", "initial time","set t0 as initial time")
                                .add_method("attach_observer", &TLimexFactory::attach_observer, "None", "end time", "set tN as endtime")
                                .add_method("create_time_integrator", &TLimexFactory::create_time_integrator, "","number of timesteps", "set N as number of timesteps")
                                .set_construct_as_smart_pointer(true);
                        reg.add_class_to_group(name_gf, "LimexFactory", tag);
                    }


                    // SimpleIntegratorFactory
                    {
                        typedef IntegratorFactory<TDomain,TAlgebra> TIntegratorFactory;
                        typedef SimpleIntegratorFactory<TDomain, TAlgebra> TBraidGridFunctionBase;
                        string name_gf = string("SimpleIntegratorFactory").append(suffix);
                        reg.add_class_<TBraidGridFunctionBase,TIntegratorFactory>(name_gf, grp) // todo constructor set by executor
                                .add_constructor()
                                .add_method("set_domain_disc", &TBraidGridFunctionBase::set_domain_disc, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_solver", &TBraidGridFunctionBase::set_solver, "None", "initial time","set t0 as initial time")
                                .add_method("set_dt_min", &TBraidGridFunctionBase::set_dt_min, "None", "end time", "set tN as endtime")
                                .add_method("set_dt_max", &TBraidGridFunctionBase::set_dt_max, "","number of timesteps", "set N as number of timesteps")
                                .add_method("set_reduction_factor", &TBraidGridFunctionBase::set_reduction_factor,"None", "t0#tN#N", "sets tstart, tstop, number of timesteps")
                                        //.add_method("setDomainDisc", &TBraidGridFunctionBase::setDomainDisc, "None", "domain discretization","set the domain")
                                .add_method("create_time_integrator", &TBraidGridFunctionBase::create_time_integrator, "None", "Gridfunction u0","set the vector for t=t0")

                                .set_construct_as_smart_pointer(true);
                        reg.add_class_to_group(name_gf, "SimpleIntegratorFactory", tag);
                    }

                    {
                        typedef IntegratorFactory<TDomain,TAlgebra> TIntegratorFactory;
                        typedef LinearTimeIntegratorFactory<TDomain, TAlgebra> TBraidGridFunctionBase;
                        string name_gf = string("LinearTimeIntegratorFactory").append(suffix);
                        reg.add_class_<TBraidGridFunctionBase,TIntegratorFactory>(name_gf, grp) // todo constructor set by executor
                                .add_constructor()
                                .add_method("set_time_disc", &TBraidGridFunctionBase::set_time_disc, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_solver", &TBraidGridFunctionBase::set_solver, "None", "initial time","set t0 as initial time")

                                        //.add_method("setDomainDisc", &TBraidGridFunctionBase::setDomainDisc, "None", "domain discretization","set the domain")
                                .add_method("create_time_integrator", &TBraidGridFunctionBase::create_time_integrator, "None", "Gridfunction u0","set the vector for t=t0")

                                .set_construct_as_smart_pointer(true);
                        reg.add_class_to_group(name_gf, "LinearTimeIntegratorFactory", tag);
                    }

                    {
                        typedef IntegratorFactory<TDomain,TAlgebra> TIntegratorFactory;
                        typedef ConstStepLinearTimeIntegratorFactory<TDomain, TAlgebra> TBraidGridFunctionBase;
                        string name_gf = string("ConstStepLinearTimeIntegratorFactory").append(suffix);
                        reg.add_class_<TBraidGridFunctionBase,TIntegratorFactory>(name_gf, grp) // todo constructor set by executor
                                .add_constructor()
                                .add_method("set_time_disc", &TBraidGridFunctionBase::set_time_disc, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_solver", &TBraidGridFunctionBase::set_solver, "None", "initial time","set t0 as initial time")

                                        //.add_method("setDomainDisc", &TBraidGridFunctionBase::setDomainDisc, "None", "domain discretization","set the domain")
                                .add_method("create_time_integrator", &TBraidGridFunctionBase::create_time_integrator, "None", "Gridfunction u0","set the vector for t=t0")
                                .add_method("set_num_steps", &TBraidGridFunctionBase::set_num_steps, "None", "Gridfunction u0","set the vector for t=t0")

                                .set_construct_as_smart_pointer(true);
                        reg.add_class_to_group(name_gf, "ConstStepLinearTimeIntegratorFactory", tag);
                    }
                    {
                        typedef IntegratorFactory<TDomain,TAlgebra> TIntegratorFactory;
                        typedef TimeIntegratorLinearAdaptiveFactory<TDomain, TAlgebra> TBraidGridFunctionBase;
                        string name_gf = string("TimeIntegratorLinearAdaptiveFactory").append(suffix);
                        reg.add_class_<TBraidGridFunctionBase,TIntegratorFactory>(name_gf, grp) // todo constructor set by executor
                                .add_constructor()
                                .add_method("set_level_factor", &TBraidGridFunctionBase::set_level_factor, "None", "verbose","set the level of verbose (true / false)")
                                .add_method("set_tol", &TBraidGridFunctionBase::set_tol, "None", "initial time","set t0 as initial time")

                                        //.add_method("setDomainDisc", &TBraidGridFunctionBase::setDomainDisc, "None", "domain discretization","set the domain")
                                .add_method("create_time_integrator", &TBraidGridFunctionBase::create_time_integrator, "None", "Gridfunction u0","set the vector for t=t0")
                                .add_method("set_time_step_max", &TBraidGridFunctionBase::set_time_step_max, "None", "Gridfunction u0","set the vector for t=t0")
                                .add_method("set_time_step_min", &TBraidGridFunctionBase::set_time_step_min, "None", "Gridfunction u0","set the vector for t=t0")
                                .add_method("set_time_stepper_1", &TBraidGridFunctionBase::set_time_stepper_1, "None", "Gridfunction u0","set the vector for t=t0")
                                .add_method("set_time_stepper_2", &TBraidGridFunctionBase::set_time_stepper_2, "None", "Gridfunction u0","set the vector for t=t0")

                                .set_construct_as_smart_pointer(true);
                        reg.add_class_to_group(name_gf, "TimeIntegratorLinearAdaptiveFactory", tag);
                    }
                }


            }


            template<typename TDomain>
            static void Domain(Registry &reg, string grp) {
                /*
                string suffix = GetDomainSuffix<TDomain>();
                string tag = GetDomainTag<TDomain>();
                 */

            }

            template<int dim>
            static void Dimension(Registry &reg, string grp) {
                /*
                string suffix = GetDimensionSuffix<dim>();
                string tag = GetDimensionTag<dim>();
                 */
            }

            template<typename TAlgebra>
            static void Algebra(Registry &reg, string grp) {
                string suffix = GetAlgebraSuffix<TAlgebra>();
                string tag = GetAlgebraTag<TAlgebra>();
            }

            static void Common(Registry &reg, string grp) { }

        };
    } // end namespace XBraidIntegrator

    extern "C" void
    InitUGPlugin_XBraidIntegrator(Registry *reg, string grp) {
        using namespace XBraidIntegrator;
        grp.append("XBraidIntegrator");
        // Space Time Communicator


        try {
            RegisterCommon<Functionality>(*reg, grp);
            RegisterDimensionDependent<Functionality>(*reg, grp);
            RegisterDomainDependent<Functionality>(*reg, grp);
            RegisterAlgebraDependent<Functionality>(*reg, grp);
            RegisterDomainAlgebraDependent<Functionality>(*reg, grp);
        }
        UG_REGISTRY_CATCH_THROW(grp);

    }
}