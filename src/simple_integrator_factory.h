//
// Created by parnet on 09.06.21.
//

#ifndef UG_PLUGIN_XBRAIDFORUG4_SIMPLE_INTEGRATOR_FACTORY_H
#define UG_PLUGIN_XBRAIDFORUG4_SIMPLE_INTEGRATOR_FACTORY_H

#include "../../Limex/time_disc/linear_implicit_timestep.h"
#include "../../Limex/time_disc/limex_integrator.hpp"

#include "../../XBraidForUG4/src/interface/integrator_factory.h"


template<typename TDomain, typename TAlgebra>
class SimpleIntegratorFactory : public IntegratorFactory<TDomain, TAlgebra>{
public:
    typedef typename TAlgebra::vector_type TVectorType;
    typedef ug::ITimeIntegrator<TDomain, TAlgebra> TTimeIntegrator;
    typedef SmartPtr<TTimeIntegrator> SPTimeIntegrator;
    typedef ug::ITimeIntegratorObserver<TDomain, TAlgebra> TObserver;
    typedef SmartPtr<TObserver> SPObserver;
    typedef ug::ISubDiagErrorEst<TVectorType> TErrorEstimator;
    typedef SmartPtr<TErrorEstimator> SPErrorEstimator;
    typedef ug::INonlinearTimeIntegrator<TDomain, TAlgebra> TIntegratorType;
    typedef typename TIntegratorType::solver_type TSolverType;
    typedef SmartPtr<TSolverType> SPSolver;
    typedef ug::IDomainDiscretization <TAlgebra> TDomainDisc;
    typedef SmartPtr <TDomainDisc> SPDomainDisc;

    typedef ug::SimpleTimeIntegrator<TDomain,TAlgebra> TSimpleTimeIntegrator;
    typedef SmartPtr<TSimpleTimeIntegrator> SPSimpleTimeIntegrator;

    typedef ug::ITimeDiscretization<TAlgebra> TTimeStepper;
    typedef SmartPtr<TTimeStepper> SPTimeStepper;

    SPObserver observer;
    SPObserver final_observer;

    double m_reduction_factor = 0.0;
    double m_dt_min = -1.0;
    double m_dt_max = -1.0;
    //SPTimeStepper m_time_stepper;
    SPDomainDisc m_domainDisc;
    SPSolver m_solver;

    SimpleIntegratorFactory() : IntegratorFactory<TDomain, TAlgebra>() {
        this->m_name = "simple integrator";
    };
    ~SimpleIntegratorFactory() = default;

    //void set_time_stepper(SPTimeStepper time_stepper){
    //    this->m_time_stepper = time_stepper;
    //}

    void set_domain_disc(SPDomainDisc dom){
        this->m_domainDisc = dom;
    }

    void set_solver(SPSolver solver){
        this->m_solver = solver;
    }


    void set_dt_min(double dt_min){
        this->m_dt_min = dt_min;
    }

    void set_dt_max(double dt_max){
        this->m_dt_max = dt_max;
    }

    void set_reduction_factor(double reduction_factor){
        this->m_reduction_factor = reduction_factor;
    }

    SPTimeIntegrator create_level_time_integrator(double current_dt, bool done, int level) override{
        //integrator->set_tol(1e-12);
        std::cout << "Delegating to create time integrator" << std::endl;
        return create_time_integrator(current_dt, done);
    }

    SPTimeIntegrator create_time_integrator(double current_dt, bool done) override{
        auto time_stepper = make_sp(new ug::LinearImplicitEuler<TAlgebra>(this->m_domainDisc));
        time_stepper->disable_matrix_cache();

        auto integrator = make_sp(new ug::SimpleTimeIntegrator<TDomain,TAlgebra>(time_stepper));
        integrator->set_solver(this->m_solver);
        integrator->set_time_step(current_dt);
        integrator->set_dt_min(current_dt);
        integrator->set_dt_max(current_dt);
        //integrator->set_tol(1e-12);
        return integrator;
    }
};
#endif //UG_PLUGIN_XBRAIDFORUG4_SIMPLE_INTEGRATOR_FACTORY_H
