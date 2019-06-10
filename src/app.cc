#include "app.hh"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

void rust_ffi::Application::OnActivated()
{
    v_table.on_activated(app);
}
void rust_ffi::Application::OnBackgroundActivated()
{
    v_table.on_background_activated(app);
}
void rust_ffi::Application::OnCachedFileUpdaterActivated()
{
    v_table.on_cached_file_updater_activated(app);
}
void rust_ffi::Application::OnFileActivated()
{
    v_table.on_file_activated(app);
}
void rust_ffi::Application::OnFileOpenPickerActivated()
{
    v_table.on_file_open_picker_activated(app);
}
void rust_ffi::Application::OnFileSavePickerActivated()
{
    v_table.on_file_save_picker_activated(app);
}
void rust_ffi::Application::OnLaunched()
{
    v_table.on_launched(app);
}
void rust_ffi::Application::OnSearchActivated()
{
    v_table.on_search_activated(app);
}
void rust_ffi::Application::OnShareTargetActivated()
{
    v_table.on_share_target_activated(app);
}
void rust_ffi::Application::OnWindowCreated()
{
    v_table.on_window_created(app);
}

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();
//    Suspending({ this, &App::OnSuspending });

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}
App::App(rust_ffi::Application* application) : App()
{
    app = application;
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const& e)
{
    app->OnLaunched();
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
{
    // Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(IInspectable const&, NavigationFailedEventArgs const& e)
{
    throw hresult_error(E_FAIL, hstring(L"Failed to load Page ") + e.SourcePageType().Name);
}

void start_app_cpp(rust_ffi::Application* app) {
    Application::Start([=](auto&&){ make<App>(app); });
}

extern "C" void start_app_ffi(rust_ffi::Application* app)
{
    start_app_cpp(app);
}
