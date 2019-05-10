
#include "../../../ATL.hpp"
#include "../../../Utilities/IO/StreamedDataFile.hpp"

template<typename T>
class ThetaLog : public atl::ObjectiveFunction<T> {
    std::vector<T> Y;
    std::vector<atl::Variable<T> > X;
    atl::Variable<T> logr0 = -2.6032947;
    atl::Variable<T> logtheta  = 0.7625692;
    atl::Variable<T> logK = T(6.0); //  = 6.7250075;
    atl::Variable<T> logQ = -4.7496015;
    atl::Variable<T> logR = -3.1889239;
    
    
public:
    
    ThetaLog() {
        
    }
    
    void Initialize() {
        
        atl::StreamedDataFile data;
        data.open("ThetaLog.dat");
        
        int size = 0;
        data >> size;
//                size = 40;
        std::cout << "size = " << size;
        
        this->Y.resize(size);
        this->X.resize(size);
        
        
        for (int i = 0; i < X.size(); i++) {
            data >> Y[i];
            //            Y[i] = tmp;
        }
        
        
        this->RegisterParameter(logr0);
        logr0.SetName("logr0");
        this->RegisterParameter(logtheta);
        logtheta.SetName("logtheta");
        this->RegisterParameter(logK);
        logK.SetName("logK");
        this->RegisterParameter(logQ);
        logQ.SetName("logQ");
        this->RegisterParameter(logR);
        logR.SetName("logR");
        
        for (int i = 0; i < X.size(); i++) {
            this->RegisterRandomVariable(X[i]);
        }
        
    }
    
    const atl::Variable<T> dnorm(const atl::Variable<T>& x,
                                 const atl::Variable<T>& mean,
                                 const atl::Variable<T>& sd, int give_log = 0) {
        if (sd.GetValue() == 0.0) {
            throw std::overflow_error("Divide by zero exception");
        }
        
        atl::Variable<T> logres = static_cast<T> (-1.0) * atl::log(T(sqrt(2 * M_PI)) * sd) - static_cast<T> (.5) * atl::pow((x - mean) / sd, static_cast<T> (2.0));
        if (give_log)return logres;
        else return atl::exp(logres);
    }
    
    virtual void Objective_Function(atl::Variable<T>& ans) {
        
              atl::Variable<T> r0 = atl::exp(logr0);
                atl::Variable<T> theta = atl::exp(logtheta);
                atl::Variable<T> K = atl::exp(logK);
                atl::Variable<T> Q = atl::exp(logQ);
                atl::Variable<T> R = atl::exp(logR);
        
        
        int timeSteps = Y.size();
        ans = T(0.0);
        
        //        atl::Variable<T> sqrtq = atl::sqrt(Q);
        atl::Variable<T> m;
        for (int i = 1; i < timeSteps; i++) {
             m = X[i - 1] +r0 * (static_cast<T> (1.0) - atl::pow(atl::exp(X[i - 1]) / K, theta));
            ans -= this->dnorm(X[i], m, atl::sqrt(Q), true);
        }
        //        atl::Variable<T> sqrtr = atl::sqrt(R);
        for (int i = 0; i < timeSteps; i++) {
            ans -= this->dnorm(atl::Variable<T>(Y[i]), X[i], atl::sqrt(R), true);
        }
        
        //return ans;
    }
    void Report() {
        std::ofstream out;
        out.open("ThetaLog.par");
        out << "logr0 = " << logr0 << "\n";
        out << "logtheta = " << logtheta << "\n";
        out << "logK = " << logK << "\n";
        out << "logQ = " << logQ << "\n";
        out << "logR = " << logR << "\n";
        out << "\nRandom Effects:\n";
        out << "X:\n";
        for (int i = 0; i < X.size(); i++) {
            out << X[i] << " ";
        }
    }
    
};


int main(int argc, char** argv) {
    
    typedef double REAL;
    
    //create the objective function
    ThetaLog<REAL> objective_function;
    
    //initialize the objective function
    objective_function.Initialize();
    
    //create an instance of a L-BFGS minimizer
    atl::LBFGS<REAL> fm;
    
    //set the objective function
    fm.SetObjectiveFunction(&objective_function);
    
    //run the minimizer
    fm.Run();
    //    
    //    //dump the results
    //    objective_function.Report();
    //    
    //    std::cout<<objective_function.GetObjectiveFunctionStatistics()<<"\n";
    return 0;
}

