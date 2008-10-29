#include "echo-client.h"
#include <iostream>
#include <pthread.h>
#include <signal.h>

#ifdef HAVE_CONFIG_H
#include <dbus-c++/config.h>
#endif

using namespace std;

static const char *ECHO_SERVER_NAME = "org.freedesktop.DBus.Examples.Echo";
static const char *ECHO_SERVER_PATH = "/org/freedesktop/DBus/Examples/Echo";

EchoClient::EchoClient(DBus::Connection &connection, const char *path, const char *name)
: DBus::ObjectProxy(connection, path, name)
{
}

void EchoClient::Echoed(const DBus::Variant &value)
{
	cout << "!";
}

/*
 * For some strange reason, libdbus frequently dies with an OOM
 */

static const int THREADS = 3;

static bool spin = true;

static DBus::Connection *conn;

void *greeter_thread(void *arg)
{
	EchoClient client(*conn, ECHO_SERVER_PATH, ECHO_SERVER_NAME);

	char idstr[16];

	snprintf(idstr, sizeof(idstr), "%lu", pthread_self());

	for (int i = 0; i < 30 && spin; ++i)
	{
		cout << client.Hello(idstr) << endl;
	}

	cout << idstr << " done " << endl;

	return NULL;
}

DBus::BusDispatcher dispatcher;

void niam(int sig)
{
	spin = false;

	dispatcher.leave();
}

int main()
{
	signal(SIGTERM, niam);
	signal(SIGINT, niam);

#ifdef DBUS_HAS_THREADS_INIT_DEFAULT
	DBus::_init_threading();
#else
	cerr << "Thread support is not enabled! your D-Bus version is too old" << endl;
#endif

	DBus::default_dispatcher = &dispatcher;

	DBus::Connection c = DBus::Connection::SessionBus();

	conn = &c;

	pthread_t threads[THREADS];

	for (int i = 0; i < THREADS; ++i)
	{
		pthread_create(threads+i, NULL, greeter_thread, NULL);
	}

	dispatcher.enter();

	cout << "terminating" << endl;

	for (int i = 0; i < THREADS; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	return 0;
}
