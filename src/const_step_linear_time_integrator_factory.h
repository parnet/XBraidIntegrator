//
// Created by parnet on 09.07.21.
//

#ifndef UG_PLUGIN_XBRAIDINTEGRATOR_CONST_STEP_LINEAR_TIME_INTEGRATOR_FACTORY_H
#define UG_PLUGIN_XBRAIDINTEGRATOR_CONST_STEP_LINEAR_TIME_INTEGRATOR_FACTORY_H

#include "../../Limex/time_disc/linear_implicit_timestep.h"
#include "../../Limex/time_disc/limex_integrator.hpp"
#include "../../Limex/time_disc/time_integrator.hpp"

#include "../../XBraidForUG4/src/interface/integrator_factory.h"

namespace ug {

    namespace XBraidIntegrator {
        template<typename TDomain, typename TAlgebra>
        class ConstStepLinearTimeIntegratorFactory : public ug::XBraidForUG4::IntegratorFactory<TDomain, TAlgebra> {
        public:
            typedef typename TAlgebra::vector_type TVectorType;
            typedef ug::ITimeIntegrator<TDomain, TAlgebra> TTimeIntegrator;
            typedef SmartPtr <TTimeIntegrator> SPTimeIntegrator;
            typedef ug::ITimeIntegratorObserver<TDomain, TAlgebra> TObserver;
            typedef SmartPtr <TObserver> SPObserver;
            typedef ug::ISubDiagErrorEst<TVectorType> TErrorEstimator;
            typedef SmartPtr <TErrorEstimator> SPErrorEstimator;
            typedef ug::INonlinearTimeIntegrator<TDomain, TAlgebra> TIntegratorType;
            typedef ug::ILinearOperatorInverse<typename TAlgebra::vector_type> TSolverType;
            typedef SmartPtr <TSolverType> SPSolver;
            typedef ug::IDomainDiscretization<TAlgebra> TDomainDisc;
            typedef SmartPtr <TDomainDisc> SPDomainDisc;

            typedef ug::SimpleTimeIntegrator<TDomain, TAlgebra> TSimpleTimeIntegrator;
            typedef SmartPtr <TSimpleTimeIntegrator> SPSimpleTimeIntegrator;

            typedef ug::ITimeDiscretization<TAlgebra> TTimeStepper;
            typedef SmartPtr <TTimeStepper> SPTimeStepper;

            SPObserver observer;
            SPObserver final_observer;


            typedef ug::ITimeDiscretization<TAlgebra> TTimeDisc;
            typedef SmartPtr <TTimeDisc> SPTimeDisc;


            ConstStepLinearTimeIntegratorFactory() : ug::XBraidForUG4::IntegratorFactory<TDomain, TAlgebra>() {
                this->m_name = "const step linear time integrator";
            };

            ~ConstStepLinearTimeIntegratorFactory() = default;

            //void set_time_stepper(SPTimeStepper time_stepper){
            //    this->m_time_stepper = time_stepper;
            //}

            SPSolver m_solver;
            SPTimeDisc m_time_disc;
            int m_num_steps = 1;

            void set_num_steps(int steps) {
                this->m_num_steps = steps;
            }

            void set_time_disc(SPTimeDisc time_disc) {
                this->m_time_disc = time_disc;
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
                        new ug::ConstStepLinearTimeIntegrator<TDomain, TAlgebra>(m_time_disc, m_solver));
                integrator->set_num_steps(this->m_num_steps);
                return integrator;
            }
        };
    }}
#endif //UG_PLUGIN_XBRAIDINTEGRATOR_CONST_STEP_LINEAR_TIME_INTEGRATOR_FACTORY_H
