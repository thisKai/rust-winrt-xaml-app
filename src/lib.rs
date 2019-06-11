use std::os::raw::c_void;

extern "C" {
    fn start_app_ffi(app: ApplicationFfi);
}

pub trait Application {
    fn start(self) where Self: Sized + 'static {
        start_app(self)
    }
    fn on_activated(&mut self) {}
    fn on_background_activated(&mut self) {}
    fn on_cached_file_updater_activated(&mut self) {}
    fn on_file_activated(&mut self) {}
    fn on_file_open_picker_activated(&mut self) {}
    fn on_file_save_picker_activated(&mut self) {}
    fn on_launched(&mut self) {}
    fn on_search_activated(&mut self) {}
    fn on_share_target_activated(&mut self) {}
    fn on_window_created(&mut self) {}
}

macro_rules! vtable_methods {
    ($fn_name: ident) => {
        extern "C" fn $fn_name(app: *mut c_void) {
            let app = app as *mut Box<Application>;
            let app = unsafe { &mut *app };
            app.$fn_name()
        }
    };
    ($($fn_name: ident,)+) => {
        $(
            vtable_methods!($fn_name);
        )+
    };
}

fn app_ffi<A: Application + 'static>(app: A) -> ApplicationFfi {
    let data: *mut Box<dyn Application> = Box::into_raw(Box::new(Box::new(app)));
    let data = data as *mut c_void;

    vtable_methods![
        on_activated,
        on_background_activated,
        on_cached_file_updater_activated,
        on_file_activated,
        on_file_open_picker_activated,
        on_file_save_picker_activated,
        on_launched,
        on_search_activated,
        on_share_target_activated,
        on_window_created,
    ];

    ApplicationFfi {
        data,
        v_table: ApplicationVTable {
            on_activated,
            on_background_activated,
            on_cached_file_updater_activated,
            on_file_activated,
            on_file_open_picker_activated,
            on_file_save_picker_activated,
            on_launched,
            on_search_activated,
            on_share_target_activated,
            on_window_created,
        },
    }
}

#[repr(C)]
struct ApplicationFfi {
    data: *mut c_void,
    v_table: ApplicationVTable,
}
#[repr(C)]
struct ApplicationVTable {
    on_activated: extern "C" fn(*mut c_void),
    on_background_activated: extern "C" fn(*mut c_void),
    on_cached_file_updater_activated: extern "C" fn(*mut c_void),
    on_file_activated: extern "C" fn(*mut c_void),
    on_file_open_picker_activated: extern "C" fn(*mut c_void),
    on_file_save_picker_activated: extern "C" fn(*mut c_void),
    on_launched: extern "C" fn(*mut c_void),
    on_search_activated: extern "C" fn(*mut c_void),
    on_share_target_activated: extern "C" fn(*mut c_void),
    on_window_created: extern "C" fn(*mut c_void),
}

fn start_app<A: Application + Sized + 'static>(app: A) {
    let app = app_ffi(app);

    unsafe {
        start_app_ffi(app);
    }
}
