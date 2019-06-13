#pragma once
#include "pch.h"

namespace rust_ffi {
    typedef void (*AppOverrideMethod)(void* app);
    struct Application {
        void OnActivated();
        void OnBackgroundActivated();
        void OnCachedFileUpdaterActivated();
        void OnFileActivated();
        void OnFileOpenPickerActivated();
        void OnFileSavePickerActivated();
        void OnLaunched();
        void OnSearchActivated();
        void OnShareTargetActivated();
        void OnWindowCreated();
    private:
        void* app;
        struct {
            AppOverrideMethod on_activated;
            AppOverrideMethod on_background_activated;
            AppOverrideMethod on_cached_file_updater_activated;
            AppOverrideMethod on_file_activated;
            AppOverrideMethod on_file_open_picker_activated;
            AppOverrideMethod on_file_save_picker_activated;
            AppOverrideMethod on_launched;
            AppOverrideMethod on_search_activated;
            AppOverrideMethod on_share_target_activated;
            AppOverrideMethod on_window_created;
        } v_table;
    };
}

template <typename D, typename ... Interfaces>
struct AppT: public ::winrt::Windows::UI::Xaml::ApplicationT<D, Interfaces...>
{
    void InitializeComponent()
    {}
};
struct App : AppT<App>
{
    App();
    App(rust_ffi::Application* application);

    void OnLaunched(::winrt::Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
    void OnSuspending(IInspectable const&, ::winrt::Windows::ApplicationModel::SuspendingEventArgs const&);
    void OnNavigationFailed(IInspectable const&, ::winrt::Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

private:
    rust_ffi::Application* app;
};

extern "C" void start_app_ffi(rust_ffi::Application* app);
