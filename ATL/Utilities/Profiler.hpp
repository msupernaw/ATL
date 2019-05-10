/*
 * Copyright 2008-2012 UnitSEA, LLC. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY UnitSEA, LLC "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL UnitSEA, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of UnitSEA, LLC.
 *
 * File:   Profiler.h
 * Author: Matthew R. Supernaw
 *
 * Created on October 17, 2011, 9:14 AM
 */

#ifndef PROFILER_H
#define PROFILER_H



//#define PROFILE


#define PROFILER_VERSION "1.0.0"




#ifdef PROFILE
#define START_APP(x) tools::ProfileApplicationStart(x)
#define END_APP(x) tools::ProfileApplicationEnd()
#define START_SCOPE(x) tools::ProfileFunctionStart(x)
#define END_SCOPE(x)  tools::ProfileFunctionEnd(x)
#define DUMP_HTML(x) tools::DumpToHtml(x)
#define LOG_EXCEPTION(x) tools::AddException(x)
#define MEMORY(x) tools::SampleMemory(x)
#define CPU(x) tools::SampleCPU(x)

#define TRY(x, y) try { \
x\
}catch(const std::logic_error& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::domain_error& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::invalid_argument& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::length_error& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::out_of_range& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::runtime_error& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::range_error& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::overflow_error& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}catch(const std::underflow_error& ia) { \
LOG_EXCEPTION(std::string(y) +std::string(ia.what()));\
}\
\

#else
#define START_APP(x)/*Not Profiling*/
#define END_APP(x) /*Not Profiling*/
#define START_SCOPE(x) /*Not Profiling*/
#define END_SCOPE(x)  /*Not Profiling*/
#define DUMP(x) /*Not Profiling*/
#define DUMP_HTML(x) /*Not Profiling*/
#define LOG_EXCEPTION(x) /*Not Profiling*/
#define MEMORY(x) /*Not Profiling*/
#define CPU(x) /*Not Profiling*/
#define TRY(x, y) x

#endif





#if defined(__linux__) || defined(__linux)|| defined(linux)
#define LINUX_OS
#elif defined(sun) || defined(__sun)
#define SOLARIS_OS
#elif defined(_AIX) || defined(__TOS_AIX__)||defined(_AIX3)|| \
       defined(AIX41)||defined(_AIX43)
#define  AIX_OS
#elif defined(__FreeBSD__)
#define  FREEBSD_OS
#elif defined(__APPLE__) || defined(__MACH__)||defined(macintosh)||\
      defined(Macintosh)
#define  DARWIN_OS
#elif defined(__NetBSD__)
#define OSTYPE NETBSD_OS
#elif defined(___OpenBSD__)
#define  OPENBSD_OS
#elif defined(_hpux) || defined(hpux) || defined(__hpux)
#define  HPUX_OS
#elif defined(sgi) || defined(__sgi)
#define  IRIX_OS
#elif defined(_WIN32) || defined(_WIN64)||defined(__WIN32__)||\
       defined(__TOS_WIN__)||defined(__WINDOWS__)||defined(_WIN32_WCE)
#define  WINDOWS_OS
#else
#define UNKNOWN_OS
#endif

#ifdef WINDOWS_OS
#include <windows.h>
#include <sys/time.h>
#include <time.h>

#define u_char unsigned char


#if defined(__MINGW32__)


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

typedef struct _MEMORYSTATUSEX {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORDLONG ullTotalPhys;
    DWORDLONG ullAvailPhys;
    DWORDLONG ullTotalPageFile;
    DWORDLONG ullAvailPageFile;
    DWORDLONG ullTotalVirtual;
    DWORDLONG ullAvailVirtual;
    DWORDLONG ullAvailExtendedVirtual;
} lMEMORYSTATUSEX;
#endif

typedef WINBOOL(WINAPI *PFN_MS_EX) (lMEMORYSTATUSEX*);

struct timepec {
    time_t tv_sec; //   seconds
    long tv_nsec; // nanoseconds
};

#else

#if defined(__GLIBC__)  // GNU Linux
#include <malloc.h>
#endif

#ifdef LINUX_OS
#include <string.h>
#include <sys/times.h>
#include <sys/vtimes.h>
#endif

#include <memory.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/sysctl.h>
#include <unistd.h>
#define u_long unsigned long
typedef unsigned char BOOL;
#define TRUE 1
#define FALSE 0
typedef long DWORD;
typedef void *LPVOID;
#endif


#ifdef DARWIN_OS
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <utility>







namespace tools {

    class ExceptionDM {
    public:

        ExceptionDM(std::string error, double time) : error_(error), time_(time) {

        }
        std::string error_;
        double time_;
    };


    class ProfiledFunction;

    static std::map<std::string, ProfiledFunction*> profiled_functions;
    static std::map<std::string, ProfiledFunction*> ::iterator profiled_functions_it;
    static std::vector<ExceptionDM*> exceptions_;

    static std::string GetPlatform() {

#if defined(DARWIN_OS)
        return "Apple";
#elif defined(LINUX_OS)
        return "Linux";
#elif defined(Solaris_OS)
        return "Solaris";
#elif defined(FREEBSD_OS)
        return "FreeBSD";
#elif defined(NETBSD_OS)
        return "NetBSD";
#elif defined(OPENBSD_OS)
        return "OpenBSD";
#elif defined(HPUX_OS)
        return "HP-UX";
#elif defined(AIX_OS)
        return "AIX";
#elif defined(IRIX_OS)
        return "IRIX";
#elif defined(WINDOWS_OS)
        return "Windows";
#else
        return "Unkown";
#endif

    }

    static unsigned int GetNumberOfProcessors() {
        int numCPU = 1;
#if defined(DARWIN_OS)
        int mib[4];
        size_t len = sizeof (numCPU);

        /* set the mib for hw.ncpu */
        mib[0] = CTL_HW;
        mib[1] = HW_AVAILCPU; // alternatively, try HW_NCPU;

        /* get the number of CPUs from the system */
        sysctl(mib, 2, &numCPU, &len, NULL, 0);

        if (numCPU < 1) {
            mib[1] = HW_NCPU;
            sysctl(mib, 2, &numCPU, &len, NULL, 0);

            if (numCPU < 1) {
                numCPU = 1;
            }
        }
        return numCPU;
#elif defined(LINUX_OS)
        numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        return numCPU;
#elif defined(Solaris_OS)
        numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        return numCPU;
#elif defined(FREEBSD_OS)
        int mib[4];
        size_t len = sizeof (numCPU);

        /* set the mib for hw.ncpu */
        mib[0] = CTL_HW;
        mib[1] = HW_AVAILCPU; // alternatively, try HW_NCPU;

        /* get the number of CPUs from the system */
        sysctl(mib, 2, &numCPU, &len, NULL, 0);

        if (numCPU < 1) {
            mib[1] = HW_NCPU;
            sysctl(mib, 2, &numCPU, &len, NULL, 0);

            if (numCPU < 1) {
                numCPU = 1;
            }
        }
        return numCPU;
#elif defined(NETBSD_OS)
        int mib[4];
        size_t len = sizeof (numCPU);

        /* set the mib for hw.ncpu */
        mib[0] = CTL_HW;
        mib[1] = HW_AVAILCPU; // alternatively, try HW_NCPU;

        /* get the number of CPUs from the system */
        sysctl(mib, 2, &numCPU, &len, NULL, 0);

        if (numCPU < 1) {
            mib[1] = HW_NCPU;
            sysctl(mib, 2, &numCPU, &len, NULL, 0);

            if (numCPU < 1) {
                numCPU = 1;
            }
        }
        return numCPU;
#elif defined(OPENBSD_OS)
        int mib[4];
        size_t len = sizeof (numCPU);

        /* set the mib for hw.ncpu */
        mib[0] = CTL_HW;
        mib[1] = HW_AVAILCPU; // alternatively, try HW_NCPU;

        /* get the number of CPUs from the system */
        sysctl(mib, 2, &numCPU, &len, NULL, 0);

        if (numCPU < 1) {
            mib[1] = HW_NCPU;
            sysctl(mib, 2, &numCPU, &len, NULL, 0);

            if (numCPU < 1) {
                numCPU = 1;
            }
        }
        return numCPU;
#elif defined(HPUX_OS)
        numCPU = mpctl(MPC_GETNUMSPUS, NULL, NULL);
        return numCPU;
#elif defined(AIX_OS)
        numCPU = sysconf(_SC_NPROCESSORS_ONLN);
        return numCPU;
#elif defined(IRIX_OS)
        numCPU = sysconf(_SC_NPROC_ONLN);
        return numCPU;
#elif defined(WINDOWS_OS)
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        numCPU = sysinfo.dwNumberOfProcessors;
        return numCPU;
#else
        return numCPU;
#endif

        return numCPU;
    }

    static double GetProcessorSpeed() {
        double Freq_Hz = -9999.0;
        //
        //#if defined(__i386) || defined(__i386)
        //        struct timeval timeBefore, timeAfter;
        //
        //        gettimeofday(&timeBefore, NULL);
        //        const uint64_t tickBefore = rdtsc();
        //
        //        sleep(10);
        //
        //        const uint64_t tickAfter = rdtsc();
        //        gettimeofday(&timeAfter, NULL);
        //
        //        const int spanInSeconds = timeAfter.tv_sec - timeBefore.tv_sec;
        //        const int spanInMicrosec = timeAfter.tv_usec - timeBefore.tv_usec;
        //
        //        Freq_Hz = static_cast<float> (tickAfter - tickBefore) / (static_cast<float> (spanInSeconds) + (static_cast<float> (spanInMicrosec) / (1000.f * 1000.f)));
        //#endif


#if defined(WINDOWS_OS)


        DWORD BufSize = _MAX_PATH;
        DWORD dwMHz = _MAX_PATH;
        HKEY hKey;

        // open the key where the proc speed is hidden:
        long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                0,
                KEY_READ,
                &hKey);

        if (lError != ERROR_SUCCESS) {// if the key is not found, tell the user why:


            return 0;
        }

        // query the key:
        RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE) & dwMHz, &BufSize);



        return static_cast<double> (dwMHz);

#elif defined(LINUX_OS)
        FILE* fp;
        char buffer[1024];
        size_t bytes_read;
        char* match;
        float clock_speed;

        /* Read the entire contents of /proc/cpuinfo into the buffer.  */
        fp = fopen("/proc/cpuinfo", "r");
        bytes_read = fread(buffer, 1, sizeof (buffer), fp);
        fclose(fp);
        /* Bail if read failed or if buffer isn't big enough.  */
        if (bytes_read == 0 || bytes_read == sizeof (buffer))
            return 0;
        /* NUL-terminate the text.  */
        buffer[bytes_read] == '\0';
        /* Locate the line that starts with "cpu MHz".  */
        match = strstr(buffer, "cpu MHz");
        if (match == NULL)
            return 0;
        /* Parse the line to extract the clock speed.  */
        sscanf(match, "cpu MHz  :  %f", &clock_speed);
        Freq_Hz = clock_speed;

#elif defined(DARWIN_OS) || defined(OPENBSD_OS) || defined(FREEBSD_OS) ||defined(NETBSD_OS)

        FILE *p = popen("sysctl -n hw.cpufrequency", "r");
        char buffer [256];
        fgets(buffer, sizeof (buffer), p);

        long long speed = atoll(buffer);

        Freq_Hz = (static_cast<double> (speed) / 1000000.0);

#endif
        return Freq_Hz;
    }




#ifdef LINUX_OS

    static clock_t lastCPU, lastSysCPU, lastUserCPU;
    static int numProcessors;

    void init() {
        FILE* file;
        struct tms timeSample;
        char line[128];


        lastCPU = times(&timeSample);
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;


        file = fopen("/proc/cpuinfo", "r");
        numProcessors = 0;
        while (fgets(line, 128, file) != NULL) {
            if (strncmp(line, "processor", 9) == 0) numProcessors++;
        }
        fclose(file);
    }

#endif



#ifdef WINDOWS_OS

    static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
    static int numProcessors;
    static HANDLE self;
    bool inited = false;
     void init() {
         inited = true;
        SYSTEM_INFO sysInfo;
        FILETIME ftime, fsys, fuser;


        GetSystemInfo(&sysInfo);
        numProcessors = sysInfo.dwNumberOfProcessors;


        GetSystemTimeAsFileTime(&ftime);
        memcpy(&lastCPU, &ftime, sizeof (FILETIME));


        self = GetCurrentProcess();
        GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
        memcpy(&lastSysCPU, &fsys, sizeof (FILETIME));
        memcpy(&lastUserCPU, &fuser, sizeof (FILETIME));
    }


#endif

    static double GetProcessorUsage() {
#ifdef WINDOWS_OS//while (1) { ps | sort -desc cpu | select -first 30; sleep -seconds 2; cls }
       
        if(!inited){
            init();
        }
        
        FILETIME ftime, fsys, fuser;
        ULARGE_INTEGER now, sys, user;
        double percent;


        GetSystemTimeAsFileTime(&ftime);
        memcpy(&now, &ftime, sizeof (FILETIME));

        GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
        memcpy(&sys, &fsys, sizeof (FILETIME));
        memcpy(&user, &fuser, sizeof (FILETIME));
  
        percent = (sys.QuadPart - lastSysCPU.QuadPart) +
                    (user.QuadPart - lastUserCPU.QuadPart);
            
           
      
            if(percent ==0){
                return 0;
            }
            
            percent /= (now.QuadPart - lastCPU.QuadPart);
            percent /= numProcessors;
            

            lastCPU = now;
            lastUserCPU = user;
            lastSysCPU = sys;
        return percent * 100;

#endif

#ifdef LINUX_OS
        struct tms timeSample;
        clock_t now;
        double percent;


        now = times(&timeSample);
        if (now <= lastCPU || timeSample.tms_stime < lastSysCPU ||
                timeSample.tms_utime < lastUserCPU) {
            //Overflow detection. Just skip this value.
            percent = -1.0;
        } else {
            percent = (timeSample.tms_stime - lastSysCPU) +
                    (timeSample.tms_utime - lastUserCPU);
            percent /= (now - lastCPU);
            percent /= numProcessors;
            percent *= 100;
        }
        lastCPU = now;
        lastSysCPU = timeSample.tms_stime;
        lastUserCPU = timeSample.tms_utime;


        return percent;
#endif

#ifdef DARWIN_OS

        pid_t pid = getpid(); //-- this is the process id you need info for
        task_t port;
        task_for_pid(mach_task_self(), pid, &port);

        task_info_data_t tinfo;
        mach_msg_type_number_t task_info_count;

        task_info_count = TASK_INFO_MAX;
        kern_return_t kr = task_info(port, TASK_BASIC_INFO, (task_info_t) tinfo, &task_info_count);
        if (kr != KERN_SUCCESS) {
            // continue; //-- skip this task
            std::cout << "ERROR\n";
        }

        task_basic_info_t basic_info;
        thread_array_t thread_list;
        mach_msg_type_number_t thread_count;

        thread_info_data_t thinfo;
        mach_msg_type_number_t thread_info_count;

        thread_basic_info_t basic_info_th;
        uint32_t stat_thread = 0; // Mach threads

        basic_info = (task_basic_info_t) tinfo;

        // get threads in the task
        kr = task_threads(port, &thread_list, &thread_count);
        if (kr != KERN_SUCCESS) {
            //   <HANDLE ERROR>
            // continue;
            std::cout << "ERROR\n";
        }
        if (thread_count > 0)
            stat_thread += thread_count;

        long tot_sec = 0;
        long tot_usec = 0;
        long tot_cpu = 0;
        int j;
        //std::cout << thread_count << "\n";
        for (j = 0; j < thread_count; j++) {
            thread_info_count = THREAD_INFO_MAX;
            kr = thread_info(thread_list[j], THREAD_BASIC_INFO,
                    (thread_info_t) thinfo, &thread_info_count);
            if (kr != KERN_SUCCESS) {
                //<HANDLE ERROR>
                continue;
            }
            basic_info_th = (thread_basic_info_t) thinfo;


            if (!(basic_info_th->flags & TH_FLAGS_IDLE)) {
                tot_sec = tot_sec + basic_info_th->user_time.seconds + basic_info_th->system_time.seconds;
                tot_usec = tot_usec + basic_info_th->system_time.microseconds + basic_info_th->system_time.microseconds;
                tot_cpu = tot_cpu + basic_info_th->cpu_usage;
            }

        } // for each thread
        //std::cout<<"Total = "<<<<"\n";
        return (tot_cpu / 10.0f);

#endif
        return 0;
    }

    static unsigned int GetAvailableRAM() {
        long ram = 0;
#ifdef DARWIN_OS
        int mib [] = {CTL_HW, HW_MEMSIZE};
        int64_t value = 0;
        size_t length = sizeof (value);

        if (sysctl(mib, 2, &value, &length, NULL, 0)) {
            ram = value;
        }

#endif


#ifdef WINDOWS_OS


        PFN_MS_EX pfnex;
        HMODULE h = GetModuleHandle("kernel32.dll");

        if (!h)
            return 0.0;

        /*  Use GlobalMemoryStatusEx if available.  */
        if ((pfnex = (PFN_MS_EX) GetProcAddress(h, "GlobalMemoryStatusEx"))) {
            lMEMORYSTATUSEX lms_ex;
            lms_ex.dwLength = sizeof lms_ex;
            if (!pfnex(&lms_ex))
                return 0.0;
            return (double) lms_ex.ullTotalPhys/(1073741824.0);
        }/*  Fall back to GlobalMemoryStatus which is always available.
                but returns wrong results for physical memory > 4GB.  */
        else {
            MEMORYSTATUS ms;
            GlobalMemoryStatus(&ms);
            return (double) ms.dwTotalPhys/(1073741824.0);   //bytes to gb
        }

#endif

    }

    int ParseLine(char* line) {
        int i = strlen(line);
        while (*line < '0' || *line > '9') line++;
        line[i - 3] = '\0';
        i = atoi(line);
        return i;
    }

    static unsigned int GetCurrentMemoryUsage() {


#ifdef WINDOWS_OS

        PFN_MS_EX pfnex;
        HMODULE h = GetModuleHandle("kernel32.dll");

        if (!h)
            return 0.0;

        /*  Use GlobalMemoryStatusEx if available.  */
        if ((pfnex = (PFN_MS_EX) GetProcAddress(h, "GlobalMemoryStatusEx"))) {
            lMEMORYSTATUSEX memInfo;
            memInfo.dwLength = sizeof memInfo;
            if (!pfnex(&memInfo))
                return 0.0;
            return (double) ((memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024.0 * 1024.0));
        }/*  Fall back to GlobalMemoryStatus which is always available.
                but returns wrong results for physical memory > 4GB.  */
        else {
            MEMORYSTATUS ms;
            GlobalMemoryStatus(&ms);
            return (double) ((ms.dwTotalPhys - ms.dwAvailPhys) / (1024.0 * 1024.0));
        }
        /*
        lMEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof (lMEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
        return (1024 * 1024)*physMemUsed;
         */
#endif


#ifdef DARWIN_OS
        task_basic_info_64_data_t info;
        unsigned size = sizeof (info);
        task_info(mach_task_self(), TASK_BASIC_INFO_64, (task_info_t) & info, &size);
        double used_memory = (info.resident_size / (1024 * 1024));
        return used_memory;
#endif

#ifdef LINUX_OS

#if defined(__GLIBC__)
        struct mallinfo meminfo;
        meminfo = mallinfo();
        //return meminfo.arena;
        return (meminfo.usmblks + meminfo.uordblks)*(1024 * 1024);
#else
        FILE* file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];


        while (fgets(line, 128, file) != NULL) {
            if (strncmp(line, "VmRSS:", 6) == 0) result = ParseLine(line);
            break;
        }
        fclose(file);
        return (result / 1024);
#endif
#endif

#if defined(OPENBSD_OS) || defined(FREEBSD_OS) ||defined(NETBSD_OS)
        PERSISTENT kvm_t* kd = kvm_open(NULL, "/dev/null", NULL, O_RDONLY, "kvm_open"); // open once
        if (kd != NULL) {
            // Use FreeBSD kvm function to get the size of resident pages (RSS).
            int procCount = 0;
            struct kinfo_proc* kp = kvm_getprocs(kd, KERN_PROC_PID, getpid(), &procCount); // do not free returned struct
            if ((kp != NULL) and(procCount >= 1)) // in case multiple threads have the same PID
                return (1024 * 1024) * kp->ki_rssize * getpagesize(); // success
        }
        return 0; // failed
#endif

        return 0;

    }

    class Timer {
    public:

        Timer() : stopped_(true) {
#if defined(WINDOWS_OS)
            QueryPerformanceFrequency(&freq_);
            start_.QuadPart = 0;
            end_.QuadPart = 0;
#else
            //  start_ = clock();
            // end_ = clock();
            ts.tv_sec = ts.tv_nsec = 0;
            te.tv_sec = te.tv_nsec = 0;
            //  startCount.tv_sec = startCount.tv_usec = 0;
            //  endCount.tv_sec = endCount.tv_usec = 0;
#endif
        }

        void Start() {
            running_ = true;
            stopped_ = false; // reset stop flag
#if defined(WIN32) || defined(WIN64)
            QueryPerformanceCounter(&start_);
#else

            ts.tv_sec = ts.tv_nsec = 0;
            te.tv_sec = te.tv_nsec = 0;

#ifdef DARWIN_OS
            clock_serv_t cclock;
            mach_timespec_t mts;
            host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
            clock_get_time(cclock, &mts);
            mach_port_deallocate(mach_task_self(), cclock);
            ts.tv_sec = mts.tv_sec;
            ts.tv_nsec = mts.tv_nsec;


#endif

#if defined(OPENBSD_OS) || defined(FREEBSD_OS) ||defined(NETBSD_OS)
            clock_gettime(CLOCK_MONOTONIC, &ts);
#endif

#if defined(LINUX_OS)
            clock_gettime(CLOCK_REALTIME, &ts);
#endif
            //gettimeofday(&startCount, NULL);
#endif
        }

        void Stop() {
            running_ = false;
            stopped_ = true; // set timer stopped flag

#ifdef WINDOWS_OS
            QueryPerformanceCounter(&end_);
#else


#ifdef DARWIN_OS
            clock_serv_t cclock;
            mach_timespec_t mts;
            host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
            clock_get_time(cclock, &mts);
            mach_port_deallocate(mach_task_self(), cclock);
            te.tv_sec = mts.tv_sec;
            te.tv_nsec = mts.tv_nsec;


#endif

#if defined(OPENBSD_OS) || defined(FREEBSD_OS) ||defined(NETBSD_OS)
            clock_gettime(CLOCK_MONOTONIC, &te);
#endif

#if defined(LINUX_OS)
            clock_gettime(CLOCK_REALTIME, &te);
#endif

            // end_ = clock();
            // gettimeofday(&endCount, NULL);
#endif
        }

#if !defined(WINDOWS_OS)

        timespec Diff(timespec start, timespec end) {

            timespec temp;
            if ((end.tv_nsec - start.tv_nsec) == 0 && (end.tv_sec - start.tv_sec) == 0) {
                temp.tv_nsec = 0;
                temp.tv_sec = 0;
                return temp;
            }


            if ((end.tv_nsec - start.tv_nsec) < 0) {
                temp.tv_sec = end.tv_sec - start.tv_sec - 1;
                temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
            } else {
                temp.tv_sec = end.tv_sec - start.tv_sec;
                temp.tv_nsec = end.tv_nsec - start.tv_nsec;
            }
            return temp;
        }
#endif

        double EllapsedTimeMilliSeconds() {
            double startTime;
            double endTime;

#if defined(WINDOWS_OS)

            if (!stopped_)
                QueryPerformanceCounter(&end_);

            startTime = start_.QuadPart * (1000000.0 / freq_.QuadPart);
            endTime = end_.QuadPart * (1000000.0 / freq_.QuadPart);
            return (endTime - startTime);
#else
            if (!stopped_) {
                end_ = clock();


#ifdef DARWIN_OS
                clock_serv_t cclock;
                mach_timespec_t mts;
                host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
                clock_get_time(cclock, &mts);
                mach_port_deallocate(mach_task_self(), cclock);
                te.tv_sec = mts.tv_sec;
                te.tv_nsec = mts.tv_nsec;


#endif

#if defined(OPENBSD_OS) || defined(FREEBSD_OS) ||defined(NETBSD_OS)
                clock_gettime(CLOCK_MONOTONIC, &te);
#endif

#if defined(LINUX_OS)
                clock_gettime(CLOCK_REALTIME, &te);
#endif


                //clock_gettime(CLOCK_MONOTONIC, &te);
                // gettimeofday(&endCount, NULL);
            }



            return ((Diff(ts, te).tv_sec * 1000.0)+(Diff(ts, te).tv_nsec / 1000000.0)); //(endTime - startTime); //deltaSeconds + deltaNSeconds;//(endTime - startTime)*.001;


#endif

        }

        bool Running() {
            return running_;
        }

    protected:

    private:

#ifdef WINDOWS_OS
        LARGE_INTEGER freq_; // ticks per second
        LARGE_INTEGER start_; //
        LARGE_INTEGER end_; //
#else
        clock_t start_; //
        clock_t end_; //
        timespec ts;
        timespec te;
        //  timeval startCount; //
        // timeval endCount;
#endif

        bool stopped_;
        bool running_;
    };



    static Timer application_timer_;
    std::string application_name_ = "Profiled Application";

    class ProfiledFunction {
    public:

        ProfiledFunction() : name_("Profiled Application"), totalTime_(0), calls_(1) {
        }

        ProfiledFunction(std::string name) : name_(name), totalTime_(0), calls_(1) {
        }

        unsigned long GetCalls_() const {
            return calls_;
        }

        void SetCalls_(unsigned long calls_) {
            this->calls_ = calls_;
        }

        std::string GetName_() const {
            return name_;
        }

        void SetName_(std::string name_) {
            this->name_ = name_;
        }

        double GetTime_() const {
            return totalTime_;
        }

        void SetTime_(double time_) {
            this->totalTime_ = time_;
        }

        void StartTiming() {
            this->start = application_timer_.EllapsedTimeMilliSeconds();
            this->calls_++;
        }

        void StopTiming() {
            this->end = application_timer_.EllapsedTimeMilliSeconds();
            this->totalTime_ += (end - start);
        }

        double GetAverageTime() {

            if (this->calls_ == 0) {
                return 0.0;
            }

            return (this->GetTime_() / this->GetCalls_());
        }


    private:
        double start;
        double end;
        std::string name_;
        double totalTime_;
        unsigned long calls_;
    };

    class MemoryDM {
    public:
        double timeStamp_;
        double ram_;
        std::string message_;

        MemoryDM() :
        timeStamp_(-9999),
        ram_(-9999),
        message_("NA") {

        }

        MemoryDM(double time, double ram, std::string message) :
        timeStamp_(time), ram_(ram),
        message_(message) {

        }

    };

    class CPUDM {
    public:
        double timeStamp_;
        double percent_;
        std::string message_;

        CPUDM() :
        timeStamp_(-9999),
        percent_(-9999),
        message_("NA") {

        }

        CPUDM(double time, double percent, std::string message) :
        timeStamp_(time), percent_(percent),
        message_(message) {

        }

    };


    std::vector<MemoryDM> ramSample_;
    std::vector<CPUDM> cpuSample_;

    static void SampleMemory(std::string message = "NA") {
        ramSample_.push_back(
                MemoryDM(application_timer_.EllapsedTimeMilliSeconds(),
                GetCurrentMemoryUsage(), message));
    }

    static void SampleCPU(std::string message = "NA") {
        cpuSample_.push_back(
                CPUDM(application_timer_.EllapsedTimeMilliSeconds(),
                GetProcessorUsage(), message));
    }

    static void ProfileApplicationStart(std::string name) {

        application_name_ = name;
        application_timer_.Start();
        std::cout << "Started Profiling Application..." << std::endl;
    }

    static void ProfileApplicationEnd() {
        application_timer_.Stop();
        std::cout << "Finished Profiling Application..." << std::endl;
    }

    static void ProfileFunctionStart(std::string name) {
        if (!application_timer_.Running()) {
            application_timer_.Start();
        }
        profiled_functions_it = profiled_functions.find(name);

        if (profiled_functions_it == profiled_functions.end()) {
            profiled_functions.insert(
                    std::pair<std::string, ProfiledFunction*>(name,
                    new ProfiledFunction(name)));
        } else {
            profiled_functions_it->second->StartTiming();
        }
    }

    static void ProfileFunctionEnd(std::string name) {
        profiled_functions_it = profiled_functions.find(name);

        if (profiled_functions_it == profiled_functions.end()) {
            std::cout << "Profiler Error: Function " << name << " not found.\n";
            exit(0);
        } else {
            profiled_functions_it->second->StopTiming();
        }
    }

    void AddException(std::string exception) {
        exceptions_.push_back(new ExceptionDM(exception,
                application_timer_.EllapsedTimeMilliSeconds()));
    }

    /**
     * Dumps data to html.
     * @param file
     */
    static void DumpToHtml(std::string file) {

        std::cout << "Writing Profiler Results..." << std::endl;
        std::ofstream out;
        double totalTime = 0;
        out.open(file.c_str());



        for (profiled_functions_it = profiled_functions.begin();
                profiled_functions_it != profiled_functions.end();
                profiled_functions_it++) {

            //total scoping time.
            totalTime += profiled_functions_it->second->GetTime_();
        }

        out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n";
        out << "<html>\n";
        out << "<head>\n";
        out << "  <title></title>\n";
        out << " <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\n";
        out << " </head>\n";
        out << " <body>\n";
        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        out << "Profiler Results: " << application_name_ <<
                "\nTotal Run Time (ms): " <<
                application_timer_.EllapsedTimeMilliSeconds() << "\n";
        out << "<br>";
        out << "<br>";
        out << "<table border=\"1\">";
        out << "<caption>Host Information</caption>";
        out << "<tr><td>Platform: </td><td>" << GetPlatform() << "</td>";
        //out << "<tr><td>CPUs: </td><td>" << GetNumberOfProcessors() << "</td>";
        out << "<tr><td>Processor: </td><td>" << GetNumberOfProcessors() << " x " << (GetProcessorSpeed() / 1000.0) << " GHz</td>";
        out << "<tr><td>Memory: </td><td>" << (1.0 * GetAvailableRAM()) << " GB</td>";
        out << "</table>";
        out << "<br>";
        out << "<table border=\"1\">\n";
        out << "<tr>";
        out << "<th>Function</th>";
        out << "<th>Calls</th>";
        out << "<th>Mean Time (ms)</th>";
        out << " <th>Percent</th>";
        out << "</tr>";
        double percent;
        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        for (profiled_functions_it = profiled_functions.begin();
                profiled_functions_it != profiled_functions.end();
                profiled_functions_it++) {


            percent = profiled_functions_it->second->GetTime_() / totalTime;
            out << "<tr>";
            out << "<td>" << profiled_functions_it->second->GetName_() << "</td><td>" <<
                    profiled_functions_it->second->GetCalls_() << "</td><td>" <<
                    profiled_functions_it->second->GetAverageTime() << "</td> <td>" <<
                    (100 * percent) << "</td>";
            out << "</tr>";
        }
        out << "</table>";
        out << "<br>";
        out << "<br>";
        out << "<table border=\"1\">\n";
        out << "<tr>";
        out << "<th>Time</th>";
        out << "<th>Exceptions Caught</th>";
        out << "</tr>";
        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        for (int i = 0; i < exceptions_.size(); i++) {
            ExceptionDM* dm = exceptions_.at(i);
            out << "<tr>";
            out << "<td>" << dm->time_ << "</td><td>" << dm->error_;
            out << "</td>";
        }
        out << "</table>";
        out << "<br>";
        out << "<br>";
        out << "<table border=\"1\">\n";
        out << "<tr>";
        out << "<th>Time</th>";
        out << "<th>Memory</th>";
        out << "<th>Message</th>";
        out << "</tr>";
        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        for (int i = 0; i < ramSample_.size(); i++) {
            out << "<tr><td>" << ramSample_.at(i).timeStamp_ <<
                    "</td><td>" << ramSample_.at(i).ram_ <<
                    "</td><td>" << ramSample_.at(i).message_ << "</td></tr>";
        }
        out << "</table>";
        out << "<br>";
        out << "<br>";
        out << "<table border=\"1\">\n";
        out << "<tr>";
        out << "<th>Time</th>";
        out << "<th>CPU Load</th>";
        out << "<th>Message</th>";
        out << "</tr>";

        for (int i = 0; i < cpuSample_.size(); i++) {
            out << "<tr><td>" << cpuSample_.at(i).timeStamp_ <<
                    "</td><td>" << cpuSample_.at(i).percent_ <<
                    "</td><td>" << cpuSample_.at(i).message_ << "</td></tr>";

        }
        out << "</table>";
        out << "</body>";
        out << "</html>";
        std::cout << "Profile Complete!\n";
        out.close();
    }

    /**
     * Dumps data to html.
     * @param file
     */
    static void DumpToCSV(std::string file) {

        std::cout << "Writing Profiler Results..." << std::endl;
        std::ofstream out;
        double totalTime = 0;

        out.open(file.c_str());


        for (profiled_functions_it = profiled_functions.begin();
                profiled_functions_it != profiled_functions.end();
                profiled_functions_it++) {

            //total scoping time.
            totalTime += profiled_functions_it->second->GetTime_();
        }

        out << "Embedded Profiler\nVersion " << PROFILER_VERSION << "\n";
        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        out << "Profiler Results: " << application_name_ <<
                "\nTotal Run Time (ms): " <<
                application_timer_.EllapsedTimeMilliSeconds() << "\n";
        out << "\n";
        out << "Host Information";
        out << "Platform:," << GetPlatform() << "\n";
        out << "Processor:, " << GetNumberOfProcessors() << " x " << (GetProcessorSpeed() / 1000.0) << " GHz\n";
        out << "Memory:," << (1.0 * GetAvailableRAM()) << " GB\n";
        out << "\n";
        out << "Function, Calls, Mean Time (ms), Percent\n";

        double percent;
        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        for (profiled_functions_it = profiled_functions.begin();
                profiled_functions_it != profiled_functions.end();
                profiled_functions_it++) {

            percent = profiled_functions_it->second->GetTime_() / totalTime;
            out << "";
            out << "" << profiled_functions_it->second->GetName_() << "," <<
                    profiled_functions_it->second->GetCalls_() << "," <<
                    profiled_functions_it->second->GetAverageTime() << "," <<
                    (100 * percent) << "\n";
        }
        out << "\n";


        out << "Time(ms), Exceptions Caught\n";

        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        for (int i = 0; i < exceptions_.size(); i++) {
            ExceptionDM* dm = exceptions_.at(i);
            out << dm->time_ << "," << dm->error_;
            out << "\n";
        }
        out << "\n";

        out << "Time(ms),Memory,Message\n";
        out.unsetf(std::ios::floatfield); // floatfield not set
        out.precision(10);
        for (int i = 0; i < ramSample_.size(); i++) {
            out << ramSample_.at(i).timeStamp_ <<
                    "," << ramSample_.at(i).ram_ <<
                    "," << ramSample_.at(i).message_ << "\n";
        }
        out << "\n";

        out << "Time,CPU Load,Message\n";

        for (int i = 0; i < cpuSample_.size(); i++) {
            out << "" << cpuSample_.at(i).timeStamp_ <<
                    "," << cpuSample_.at(i).percent_ <<
                    "," << cpuSample_.at(i).message_ << "\n";
        }
        out << "\n";
        out.close();
        std::cout << "Profile Complete!\n";
    }
}

#endif  /* PROFILER_H */
