#include <QApplication>
#include <QDBusConnection>

#include "agent.h"
#include "agentadaptor.h"

int main(int argc, char *argv[])
{
    return Agent(argc, argv).exec();
}
