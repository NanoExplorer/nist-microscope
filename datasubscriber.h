#ifndef DATASUBSCRIBER_H
#define DATASUBSCRIBER_H

#include <zmq.hpp>
#include <QObject>
#include <QVector>

class QThread;
class plotWindow;
class refreshPlots;
class pulseRecord;

///////////////////////////////////////////////////////////////////////////////
/// Object to run in a private thread,
///
class dataSubscriber : public QObject
{
    Q_OBJECT

public:
    dataSubscriber(plotWindow *w, zmq::context_t *zmqcontext, std::string tcpsource);
    ~dataSubscriber();

    void wait(unsigned long time=ULONG_MAX);

signals:
    /// Signal emitted when this object completes (i.e., its destructor is called)
    void finished(void);

    void newSampleTime(double);

    void newRecordLengths(int,int);

    /// Signal that a data vector is ready to plot
    void newDataToPlot(int channum, pulseRecord *pr);


public slots:
    /// Slot to call when it's time to terminate this thread.
    void terminate(void) {emit finished();}
    void process(void);

private:
    QThread *myThread;  ///< The QThread where this object's work is performed
    plotWindow   *window;      ///< Where we the data are plotted
    refreshPlots *plotManager; ///< Where we send data for plotting
    zmq::context_t *zmqcontext;
    zmq::socket_t *subscriber;
    zmq::socket_t *killsocket;
    zmq::socket_t *chansocket;
    std::string tcpdatasource;

    void subscribeChannel(int channum);
    void unsubscribeChannel(int channum);
    void parseChannelMessage(zmq::message_t &);

    double sampletime;
    int nsamples;
    int presamples;
};


#endif // DATASUBSCRIBER_H
