#include "MainWindow.h"
#include "EnergyInfoProvider.h"
#include "Exception.h"
#include "AtExit.h"
#include "EngFormattedValue.h"

using namespace std;

extern "C" {
	void quitIndicator(GtkMenu *, gpointer);
}

void quitIndicator(GtkMenu *menu, gpointer data) {
	Q_UNUSED(menu);
	QApplication *self = static_cast<QApplication *>(data);

	self->quit();
}

void showMainWindow(GtkMenu *menu, gpointer data) {
	Q_UNUSED(menu);
	MainWindow *wnd = static_cast<MainWindow *>(data);
	QDesktopWidget desktop;
	QRect scrSize = desktop.availableGeometry(wnd);
	// put at top right corner
	auto coords = scrSize.topRight();
	coords.rx() -= wnd->width();
	wnd->move(coords);
	wnd->show();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

//	int b = 1;
//	while(b){  // wait for debugger to attach
//		std::this_thread::sleep_for( std::chrono::seconds(1) );
//	}

	try{
		EngFormattedValue::loadPrefixes();
		EnergyInfoProvider::init();
		AtExit closeEnergyInfo([]{
			EnergyInfoProvider::terminate();
		});

		PowerMeter pm;
		MainWindow w;
		w.powerMeter(&pm);


		QString desktop = getenv("XDG_CURRENT_DESKTOP");
		if (desktop.toLower() == "unity") {
			AppIndicator *indicator;
			GtkWidget *menu, *item;

			menu = gtk_menu_new();
			{
				item = gtk_menu_item_new_with_label(QApplication::tr("Show plot").toUtf8());
				g_signal_connect(item, "activate",
												 G_CALLBACK(showMainWindow), &w);
				gtk_widget_show(item);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
			}
			{
				item = gtk_menu_item_new_with_label(QApplication::tr("Quit").toUtf8());
				g_signal_connect(item, "activate",
												 G_CALLBACK(quitIndicator), qApp);
				gtk_widget_show(item);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
			}

			indicator = app_indicator_new(
						"indicator-cpu-power",
						"indicator-processor",
						APP_INDICATOR_CATEGORY_APPLICATION_STATUS
						);
			app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
			app_indicator_set_menu(indicator, GTK_MENU(menu));
			QObject::connect(&pm, &PowerMeter::power,
				[indicator](std::vector<double> &powerMP, std::vector<double> &powerGPU, double dt) {
					Q_UNUSED(powerGPU);
					Q_UNUSED(dt);
					double power = std::accumulate(begin(powerMP), end(powerMP), 0.);
					QString status = QString(QApplication::tr("%1W").arg(EngFormattedValue::f(power, 3)));
					app_indicator_set_label(indicator, status.toUtf8().data(), "");
			});
		}
		else{
			w.show();
		}
		return a.exec();
	}
	catch(std::exception &e){
		QMessageBox::critical(
			nullptr,
			QApplication::tr("Error"),
			Message(e));
	}
	return -1;
}
