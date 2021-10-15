//
// Created by parnet on 09.07.21.
//

#ifndef UG_PLUGIN_XBRAIDINTEGRATOR_LINEAR_TIME_INTEGRATOR_FACTORY_H
#define UG_PLUGIN_XBRAIDINTEGRATOR_LINEAR_TIME_INTEGRATOR_FACTORY_H


#include "../../Limex/time_disc/linear_implicit_timestep.h"
#include "../../Limex/time_disc/limex_integrator.hpp"

#include "../../XBraidForUG4/src/interface/integrator_factory.h"

namespace ug {

    namespace XBraidIntegrator {
        template<typename TDomain, typename TAlgebra>
        class LinearTimeIntegratorFactory : public ug::XBraidForUG4::IntegratorFactory<TDomain, TAlgebra> {
        public:
            typedef typename TAlgebra::vector_type TVectorType;

            typedef ug::ITimeIntegrator<TDomain, TAlgebra> TTimeIntegrator;
            typedef SmartPtr <TTimeIntegrator> SPTimeIntegrator;

            typedef ug::ITimeIntegratorObserver<TDomain, TAlgebra> TObserver;
            typedef SmartPtr <TObserver> SPObserver;

            typedef ug::ISubDiagErrorEst<TVectorType> TErrorEstimator;
            typedef SmartPtr <TErrorEstimator> SPErrorEstimator;


            typedef ug::ILinearOperatorInverse<typename TAlgebra::vector_type> TSolverType;
            typedef SmartPtr <TSolverType> SPSolver;
            typedef ug::IDomainDiscretization<TAlgebra> TDomainDisc;
            typedef SmartPtr <TDomainDisc> SPDomainDisc;

            typedef ug::ITimeDiscretization<TAlgebra> TTimeDisc;
            typedef SmartPtr <TTimeDisc> SPTimeDisc;

            typedef ug::SimpleTimeIntegrator<TDomain, TAlgebra> TSimpleTimeIntegrator;
            typedef SmartPtr <TSimpleTimeIntegrator> SPSimpleTimeIntegrator;


            SPObserver observer;
            SPObserver final_observer;

            //SPTimeStepper m_time_stepper;
            SPSolver m_solver;
            SPTimeDisc m_time_disc;


            LinearTimeIntegratorFactory() : ug::XBraidForUG4::IntegratorFactory<TDomain, TAlgebra>() {
                this->m_name = "linear time integrator";
            };

            ~LinearTimeIntegratorFactory() = default;

            //void set_time_stepper(SPTimeStepper time_stepper){
            //    this->m_time_stepper = time_stepper;
            //}

            void set_time_disc(SPTimeDisc tdisc) {
                this->m_time_disc = tdisc;
            }

            void set_solver(SPSolver solver) {
                this->m_solver = solver;
            }

            SPTimeIntegrator create_level_time_integrator(double current_dt, bool done, int level) override {
                //integrator->set_tol(1e-12);
                std::cout << "Delegating to create time integrator" << std::endl;
                return create_time_integrator(current_dt, done);
            }

            SPTimeIntegrator create_time_integrator(double current_dt, bool done) override {
                auto integrator = make_sp(
                        new ug::LinearTimeIntegrator<TDomain, TAlgebra>(this->m_time_disc, this->m_solver));
                integrator->set_time_step(current_dt);

                return integrator;
            }
        };
    }}

#endif //UG_PLUGIN_XBRAIDINTEGRATOR_LINEAR_TIME_INTEGRATOR_FACTORY_H
