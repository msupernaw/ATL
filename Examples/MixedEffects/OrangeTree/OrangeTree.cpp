#include "../../../ATL/ATL.hpp"
#include "../../../ATL/Utilities/IO/StreamedDataFile.hpp"


template<typename T>
class OrangeTree : public atl::ObjectiveFunction<T> {
public:
    
    
    typedef atl::Variable<T> variable;
    int M;
    int N;
    std::valarray<T> x;
    std::valarray<std::valarray<T> > y;
    
    variable phi1;
    variable phi2;
    variable phi3;
    variable logSigma;
    variable logSigmaB;
    
    variable sigma;
    variable sigmaB;
    
    std::vector<variable> b;
    std::valarray<std::valarray<variable> > yfit;
    
    variable RSS;
    
    virtual void Initialize() {
        
        atl::StreamedDataFile data;
        data.open("orange.dat");
        
        atl::StreamedDataFile pin;
        pin.open("orange.pin");
        data >> M;
        data >> N;
        
        //        phi1 = 200.0;
        pin>>phi1;
        //        phi2 = 800.0;
        pin>>phi2;
        //        phi3 = 400.0;
        pin>>phi3;
        //        logSigma = 1.0;
        pin>>logSigma;
        //        logSigmaB = 0.0;
        pin>>logSigmaB;
        
        
        
        
        this->RegisterParameter(logSigmaB, 3);
        logSigmaB.SetName("logSigmaB");
        this->RegisterParameter(phi1);
        phi1.SetName("phi1");
        this->RegisterParameter(phi2);
        phi2.SetName("phi2");
        this->RegisterParameter(phi3);
        phi3.SetName("phi3");
        this->RegisterParameter(logSigma);
        logSigma.SetName("logSigma");
        
        x.resize(N);
        for (int i = 0; i < N; i++) {
            data >> x[i];
        }
        b.resize(M);
        for (int i = 0; i < M; i++) {
            b[i] = 0.000; //variable(0.000);
        }
        
        for (int i = 0; i < M; i++) {
            std::stringstream ss;
            ss << "B[" << i << "]";
            b[i].SetName(ss.str());
            this->RegisterRandomVariable(b[i], 2);
        }
        
        
        y.resize(M);
        yfit.resize(M);
        for (int i = 0; i < M; i++) {
            y[i] = std::valarray<T>(N);
            yfit[i] = std::valarray<variable>(N);
            for (int j = 0; j < N; j++) {
                data >> y[i][j];
            }
        }
        
    }
    
   
    
    const std::valarray<variable> square(const std::valarray<variable>& v) {
        std::valarray<variable> ret(v.size());
        for (int i = 0; i < v.size(); i++) {
            ret[i] = v[i] * v[i];
        }
        return ret;
    }
    
    const std::vector<variable> square(const std::vector<variable>& v) {
        std::vector<variable> ret(v.size());
        for (int i = 0; i < v.size(); i++) {
            ret[i] = v[i] * v[i];
        }
        return ret;
    }
    
    const variable sum(const std::valarray<variable>& v) {
        variable sum = 0.0;
        for (int i = 0; i < v.size(); i++) {
            sum += v[i];
        }
        return sum;
    }
    
    const variable sum(const std::vector<variable>& v) {
        variable sum = 0.0;
        for (int i = 0; i < v.size(); i++) {
            sum += v[i];
        }
        return sum;
    }
    
    template<typename TT>
    TT sum(const std::valarray<std::valarray<TT> >& v) {
        TT sum = 0.0;
        for (int i = 0; i < v.size(); i++) {
            for (int j = 0; j < v[0].size(); j++) {
                sum += v[i][j];
            }
        }
        return sum;
    }
    
    const std::valarray<std::valarray<variable> > subtract(const std::valarray<std::valarray<T> >& a, const std::valarray<std::valarray<variable> >& b) {
        std::valarray<std::valarray<variable> > ret;
        ret.resize(a.size(), std::valarray<variable>(a[0].size()));
        
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < a[0].size(); j++) {
                ret[i][j] = a[i][j] - b[i][j];
            }
        }
        return ret;
    }
    
    const std::valarray<atl::Variable<T> > dnorm(const std::vector<atl::Variable<T> > & x,
                                 const atl::Variable<T>& mean,
                                 const atl::Variable<T>& sd, int give_log = 0) {
        if (sd.GetValue() == 0.0) {
            throw std::overflow_error("Divide by zero exception");
        }
        std::valarray<atl::Variable<T> > ret(x.size());
        for(int i =0; i < x.size(); i++){
            ret[i] = variable();
            ret[i] = static_cast<T> (-1.0) * atl::log(T(sqrt(2 * M_PI)) * sd) - static_cast<T> (.5) * atl::pow((x[i] - mean) / sd, static_cast<T> (2.0));
            if (!give_log){
                ret[i] = atl::exp(ret[i]);
            }
            
        }
        
        return ret;
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
    
    virtual const atl::Variable<T> Evaluate() {
        int M = y.size();  // number of individuals
        int n = y[0].size();  // number of time steps
        variable jnll = 0.0;
        sigma = atl::exp(logSigma);
        sigmaB = atl::exp(logSigmaB);
        
        for (int i=0; i<M; i++)  // individuals
        {
            for (int j=0; j<n; j++)  // time steps
            {
                yfit[i][j] = (phi1 + b[i]) / (1.0 + atl::exp(-1.0*(x[j]-phi2)/phi3));
                jnll += -dnorm(y[i][j], yfit[i][j], sigma, true);
            }
        }
        jnll += -sum(dnorm(b, variable(0.0), sigmaB, true));
//        variable f = 0.0;
//
//
//        sigma = atl::exp(logSigma);
//        sigmaB = atl::exp(logSigmaB);
//
//
//        RSS = 0;
//        RSS.SetName("RSS");
//        for (int i = 0; i < M; i++) {
//            for (int j = 0; j < N; j++) {
//                yfit[i][j] = (phi1 + b[i]) / (1.0 + atl::exp(-1.0 * (x[j] - phi2) / phi3));
//                RSS += (y[i][j] - yfit[i][j])*(y[i][j] - yfit[i][ j]);
//            }
//        }
//        variable sumb = sum(square(b));
//        f = 0.5 * M * N * log(2.0 * M_PI) + T(M * N) * logSigma + RSS / (2.0 * (sigma * sigma));
//        f += 0.5 * M * log(2.0 * M_PI) + T(M) * logSigmaB + sumb / (2.0 * (sigmaB * sigmaB));
//
        return jnll;
    }
    
    virtual void Objective_Function(atl::Variable<T>& f) {
        f = this->Evaluate();
    }
    
    void Report(){
        
    }
    
};

int main(int argc, char** argv) {
    
    typedef double  REAL;

    //create the objective function 
    OrangeTree<REAL> objective_function;
    
    //initialize the objective function
    objective_function.Initialize();
    
    //create an instance of a L-BFGS minimizer
    atl::LBFGS<REAL> fm;
    
    //set the objective function
    fm.SetObjectiveFunction(&objective_function);
    
    //set output width to 2
    fm.SetPrintWidth(2);
    
    //run the minimizer
    fm.Run();
    
    //dump the results
    objective_function.Report();
    return 0;
}

