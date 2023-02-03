
#ifndef HYBRIDAPI_SIMULATION_H
#define HYBRIDAPI_SIMULATION_H

#include <ctime>


template <class OutputDataType, class InputDataType>
class Simulation
{
private:

    // How often (seconds to wait) should the state of the simulation be reported to the virtual environment.
    float mVirtualEnvironmentReportPeriod = 0.5;

    // If this mode is enabled, the simulation will have to explicitly indicate that a new state should be
    // reported to the virtual environment when possible. By default it is disabled, and the simulation state
    // is reported continuously. When it is enabled, it can be marked through the requestReportState() API.
    bool mReportWhenMarked = false;

    std::clock_t mLastUpdate;
    std::clock_t mLastReportUpdate;

protected:

    bool mShouldReportInReportWhenMarkedMode = false;

    /**
     * Retrieves a request or interaction from the virtual environment, if any is available. If one was available
     * then the function returns true and will have copied the data to the structure. Otherwise it will return false
     * and copy nothing. Retrieved items are removed from the queue and never returned again.
     * - Design question: How many requests can we have? Is it a queue? Is the number limited? Do they expire?
     * - Design question: Should we instead have a check for how many are available, and throw an exception if it fails
     * reading? Or if the STL is available, should we instead return an Optional or similar?
     * @return
     */
    bool readRequest(InputDataType &request) {
        // TO-DO: Implement.
        strncpy(request.my_string, "nSFd0,b0;nb0b1;ab2g31g14;ob1LTb2;\n", sizeof(request.my_string) - 1);
        // request.my_string = "nSFd0,b0;nb0b1;ab2g31g14;ob1LTb2;\n";
        return true;

        // return false;
    }

    /**
     * Enables and disables the report when marked mode.
     * @param reportWhenMarked
     */
    void setReportWhenMarked(bool reportWhenMarked) {
        mReportWhenMarked = reportWhenMarked;
    }

    /**
     * Gets the report when marked mode state.
     * @return
     */
    bool getReportWhenMarked() {
        return mReportWhenMarked;
    }

    /**
     * Sets the virtual environment report period.
     * @param period Period to set it to, in seconds.
     */
    void setVirtualEnvironmentReportPeriod(float period) {
        mVirtualEnvironmentReportPeriod = period;
    }

    /**
     * Retrieves the currently set virtual environment report period.
     * @return
     */
    float getVirtualEnvironmentReportPeriod() {
        return mVirtualEnvironmentReportPeriod;
    }

    /**
     * Queues the current state to be sent to the virtual environment. This should normally NOT be called
     * explicitly by the specific simulation developer; it should be called periodically on its own, from their
     * perspective.
     */
    void sendVirtualEnvironmentReport() {
        // TO-DO: Implement.
        std::cout << "DBG: " << "sending virtual environment report" << std::endl;
    }

    /**
     * This only takes an effect in the report-when-marked mode. In this mode, the simulation can notify that the
     * state of the simulation is ready to be reported to the virtual environment by calling this method. When called,
     * the report will not be sent straightaway, but it will be queued in the next report cycle.
     */
    void requestReportState() {
        mShouldReportInReportWhenMarkedMode = true;
    }

public:

    /**
     * The part of the simulation-specific state that will be serialized periodically and transmitted to the
     * virtual environment. This state may be changed freely by the simulation. The state will only be propagated
     * periodically to the virtual environment, not after every single change.
     *
     * Note: Should probably be protected, but it is public for debugging purposes.
     */
    OutputDataType mState;

    /**
     * This internal method is meant to be invoked externally as often as possible, and it is not meant to be
     * invoked by specific simulation developers. Calling the method will ensure that the update() and reportUpdate()
     * methods be invoked periodically.
     */
    void _update() {
        std::clock_t currentClock = clock();
        _update(currentClock);
    }

    /**
     * This internal method is meant to be invoked externally as often as possible, and it is not meant to be
     * invoked by specific simulation developers. Calling the method will ensure that the update() and reportUpdate()
     * methods be invoked periodically.
     *
     * This version of the method accepts the current clock time as an argument. This way simulation developers may
     * test the simulation updates more freely and make the simulation run faster than the processor's clock.
     * That is, to run faster than it normally would.
     *
     * @param clk Current clock()
     */
    void _update(std::clock_t currentClock) {
        std::clock_t elapsedUpdate = currentClock - mLastUpdate;
        std::clock_t elapsedReportUpdate = currentClock - mLastReportUpdate;

        update(elapsedUpdate / (double)CLOCKS_PER_SEC);
        mLastUpdate = currentClock;

        if(elapsedReportUpdate / (double)CLOCKS_PER_SEC > mVirtualEnvironmentReportPeriod) {
            reportUpdate();
            mLastReportUpdate = currentClock;
        }
    }

    /**
     * Internal method to be invoked to initialize the simulation. It will conduct some initial housekeeping and
     * then invoke the virtual initialize() function that simulation developers are meant to override.
     */
    void _initialize() {
        mLastUpdate = clock();
        mLastReportUpdate = clock();

        initialize();
    }

    /**
     * Invoked periodically and very often by the system. This function is meant to be override so as to update the state of the
     * simulation. This is meant to be called with a high-frequency, and generally it should not block. Updates to the
     * simulation's state during the execution of this function will not necessarily be propagated immediately to the
     * server. This function may read from the GPIOs or other hardware devices.
     *
     * @param delta Time that has elapsed since this function was called, in seconds.
     */
    virtual void update(double delta) {
    }

    /**
     * Invoked periodically by the system to send the simulation's state to the environment/external system. The frequecy
     * at which this is run should be the mVirtualEnvironmentReportPeriod. If in reportWhenMarked mode, then it will only
     * queue the data to be sent if the flag was set through the requestReportState API.
     */
    virtual void reportUpdate() {
        if(mReportWhenMarked) {
            if(mShouldReportInReportWhenMarkedMode) {
                sendVirtualEnvironmentReport();
            }
        } else {
            sendVirtualEnvironmentReport();
        }

        mShouldReportInReportWhenMarkedMode = false;
    }

    /**
     * To initialize the simulation.
     */
    virtual void initialize() {

    }
};

#endif //HYBRIDAPI_SIMULATION_H
