#include <gtkmm.h>
#include <iostream>

namespace {
Gtk::Window *pMain = nullptr;
Glib::RefPtr<Gtk::Application> app;

void on_app_activate() {
    // Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("res/test.ui");
    } catch (const Glib::FileError &ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return;
    } catch (const Glib::MarkupError &ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return;
    } catch (const Gtk::BuilderError &ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return;
    }

    // Get the GtkBuilder-instantiated dialog:
    refBuilder->get_widget("MainWindow", pMain);
    if (!pMain) {
        std::cerr << "Could not get the MainWindow" << std::endl;
        return;
    }

    // It's not possible to delete widgets after app->run() has returned.
    // Delete the dialog with its child widgets before app->run() returns.
    pMain->signal_hide().connect([]() { delete pMain; });

    app->add_window(*pMain);
    pMain->set_visible(true);
}
} // anonymous namespace

int main(int argc, char **argv) {
    app = Gtk::Application::create("org.gtkmm.example");

    // Instantiate a dialog when the application has been activated.
    // This can only be done after the application has been registered.
    // It's possible to call app->register_application() explicitly, but
    // usually it's easier to let app->run() do it for you.
    app->signal_activate().connect([]() { on_app_activate(); });

    return app->run(argc, argv);
}
