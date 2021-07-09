//
// Created by parnet on 09.07.21.
//

#ifndef UG_PLUGIN_XBRAIDINTEGRATOR_TIME_INTEGRATOR_LINEAR_ADAPTIVE_FACTORY_H
#define UG_PLUGIN_XBRAIDINTEGRATOR_TIME_INTEGRATOR_LINEAR_ADAPTIVE_FACTORY_H

#include "../../Limex/time_disc/linear_implicit_timestep.h"
#include "../../Limex/time_disc/limex_integrator.hpp"

#include "../../XBraidForUG4/src/interface/integrator_factory.h"

template<typename TDomain, typename TAlgebra>
class TimeIntegratorLinearAdaptiveFactory : public IntegratorFactory<TDomain, TAlgebra>{
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

    SPTimeStepper m_time_disc_1;
    SPTimeStepper m_time_disc_2;

    double m_tol = 1e-2;
    double m_dt_min = -1.0;
    double m_dt_max = -1.0;
    double level_factor = 1.0;

    TimeIntegratorLinearAdaptiveFactory() : IntegratorFactory<TDomain, TAlgebra>() {
        this->m_name = "time integrator linear adaptive";
    };

    ~TimeIntegratorLinearAdaptiveFactory() = default;

    //void set_time_stepper(SPTimeStepper time_stepper){
    //    this->m_time_stepper = time_stepper;
    //}

    void set_time_stepper_1(SPTimeStepper time_disc){
        this->m_time_disc_1 = time_disc;
    }

    void set_time_stepper_2(SPTimeStepper time_disc){
        this->m_time_disc_2 = time_disc;
    }

    void set_time_step_min(double dt){
        this->m_dt_min = dt;
    }

    void set_time_step_max(double dt){
        this->m_dt_max = dt;
    }

    void set_tol(double tol){
        this->m_tol = tol;
    }

    void set_level_factor(double factor){
        this->level_factor = factor;
    }

    SPTimeIntegrator create_level_time_integrator(double current_dt, bool done, int level) override{
        //integrator->set_tol(1e-12);
        std::cout << "Delegating to create time integrator" << std::endl;
        auto integrator = make_sp(new ug::TimeIntegratorLinearAdaptive<TDomain,TAlgebra>(m_time_disc_1,m_time_disc_2));
        integrator->set_time_step_max(this->m_dt_max);
        integrator->set_time_step_min(this->m_dt_min);
        integrator->set_tol(this->m_tol);

        double factor = 1.0;
        if(level > 0 ){
            factor = pow(this->level_factor,level);
        }

        integrator->set_tol(this->m_tol*factor);
        return integrator;
    }

    SPTimeIntegrator create_time_integrator(double current_dt, bool done) override{
        auto integrator = make_sp(new ug::TimeIntegratorLinearAdaptive<TDomain,TAlgebra>(m_time_disc_1,m_time_disc_2));
        integrator->set_time_step_max(this->m_dt_max);
        integrator->set_time_step_min(this->m_dt_min);
        integrator->set_tol(this->m_tol);
        return integrator;
    }
};
#endif //UG_PLUGIN_XBRAIDINTEGRATOR_TIME_INTEGRATOR_LINEAR_ADAPTIVE_FACTORY_H
