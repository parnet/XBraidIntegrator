//
// Created by parnet on 09.06.21.
//

#ifndef UG_PLUGIN_XBRAIDFORUG4_LIMEX_INTEGRATOR_FACTORY_H
#define UG_PLUGIN_XBRAIDFORUG4_LIMEX_INTEGRATOR_FACTORY_H

#include "../../Limex/time_disc/linear_implicit_timestep.h"
#include "../../Limex/time_disc/limex_integrator.hpp"

#include "../../XBraidForUG4/src/interface/integrator_factory.h"
namespace ug {

    namespace XBraidIntegrator {
        template<typename TDomain, typename TAlgebra>
    class LimexFactory : public ug::XBraidForUG4::IntegratorFactory<TDomain, TAlgebra> {
        public:
            typedef typename TAlgebra::vector_type TVectorType;
            typedef ug::ITimeIntegrator<TDomain, TAlgebra> TTimeIntegrator;
            typedef SmartPtr <TTimeIntegrator> SPTimeIntegrator;
            typedef ug::ITimeIntegratorObserver<TDomain, TAlgebra> TObserver;
            typedef SmartPtr <TObserver> SPObserver;
            typedef ug::ISubDiagErrorEst<TVectorType> TErrorEstimator;
            typedef SmartPtr <TErrorEstimator> SPErrorEstimator;
            typedef ug::INonlinearTimeIntegrator<TDomain, TAlgebra> TIntegratorType;
            typedef typename TIntegratorType::solver_type TSolverType;
            typedef SmartPtr <TSolverType> SPSolver;
            typedef ug::IDomainDiscretization<TAlgebra> TDomainDisc;
            typedef SmartPtr <TDomainDisc> SPDomainDisc;

            SPObserver observer;
            SPObserver final_observer;


            SPDomainDisc domain_disc;
            SPSolver solver;
            SPErrorEstimator error_est;

            int n_stages = 4;
            std::vector<int> steps = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            double tol = 1e-3;
            double dt;
            double dt_min = -1;  // 1e-2 * 1e-2*char time;
            double dt_max = -1;  // char time
            double safety_factor = 0.25;
            double increase_factor = 2.0;
            double greedy_order_factor = 0.0;
            bool disable_matrix_cache = true;

            double level_factor = 0.5;

            LimexFactory() : ug::XBraidForUG4::IntegratorFactory<TDomain, TAlgebra>() {
                this->m_name = "limex integrator";
            };

            ~LimexFactory() = default;

            void set_domain_disc(SPDomainDisc domain_disc) {
                this->domain_disc = domain_disc;
            }

            void set_solver(SPSolver solver) {
                this->solver = solver;
            }

            void set_dt_min(double dt) {
                this->dt_min = dt;
            }

            void set_dt_max(double dt) {
                this->dt_max = dt;
            }

            void set_tol(double tol) {
                this->tol = tol;
            }

            void set_error_estimator(SPErrorEstimator error_est) {
                this->error_est = error_est;
            }

            void attach_observer(SPObserver observer) {
                this->observer = observer;
            }

            void set_level_factor(double factor) {
                this->level_factor = factor;
            }

            SPTimeIntegrator create_level_time_integrator(double current_dt, bool done, int level) {
                auto limex = make_sp(new ug::LimexTimeIntegrator<TDomain, TAlgebra>(this->n_stages));
                for (int i = 0; i < this->n_stages; i++) {
                    limex->add_stage(this->steps[i], this->solver, this->domain_disc);
                }
                limex->add_error_estimator(this->error_est);
                double factor = 1.0;

                if (level > 0) {
                    factor = pow(this->level_factor, level);
                }

                double curfactor = this->tol * factor;
                std::cout << "XBRAID-LIMEX-TOL " << level << " : " << curfactor << std::endl;

                limex->set_tolerance(curfactor);
                limex->set_time_step(current_dt);

                if (this->dt_min == -1) {
                    limex->set_time_step(current_dt * 1e-2);
                } else {
                    limex->set_time_step(this->dt_min);
                }

                if (this->dt_max == -1) {
                    limex->set_time_step(current_dt);
                } else {
                    limex->set_time_step(this->dt_max);
                }

                if (this->disable_matrix_cache) {
                    limex->disable_matrix_cache();
                } else {
                    limex->enable_matrix_cache();
                }

                limex->set_stepsize_safety_factor(this->safety_factor);
                limex->set_increase_factor(this->increase_factor);
                limex->set_stepsize_greedy_order_factor(this->greedy_order_factor);

                //limex->attach_observer();
                return limex;
            }

            SPTimeIntegrator create_time_integrator(double current_dt, bool done) override {
                auto limex = make_sp(new ug::LimexTimeIntegrator<TDomain, TAlgebra>(this->n_stages));
                for (int i = 0; i < this->n_stages; i++) {
                    limex->add_stage(this->steps[i], this->solver, this->domain_disc);
                }
                limex->add_error_estimator(this->error_est);
                limex->set_tolerance(this->tol);
                limex->set_time_step(current_dt);

                if (this->dt_min == -1) {
                    limex->set_time_step(current_dt * 1e-2);
                } else {
                    limex->set_time_step(this->dt_min);
                }

                if (this->dt_max == -1) {
                    limex->set_time_step(current_dt);
                } else {
                    limex->set_time_step(this->dt_max);
                }

                if (this->disable_matrix_cache) {
                    limex->disable_matrix_cache();
                } else {
                    limex->enable_matrix_cache();
                }

                limex->set_stepsize_safety_factor(this->safety_factor);
                limex->set_increase_factor(this->increase_factor);
                limex->set_stepsize_greedy_order_factor(this->greedy_order_factor);

                //limex->attach_observer();
                return limex;
            }
        };
    }}

#endif //UG_PLUGIN_XBRAIDFORUG4_LIMEX_INTEGRATOR_FACTORY_H
