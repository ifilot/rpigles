#include "display.h"

Display::Display() {
    this->initialize_opengles();
}

void Display::initialize_opengles() {
    bcm_host_init();

    int32_t success = 0;
    EGLBoolean result;
    EGLint num_config;

    bcm_host_init();

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T  dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    static const EGLint context_attributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLConfig config;

    // get an EGL display connection
    this->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    // initialize the EGL display connection
    result = eglInitialize(this->display, NULL, NULL);

    // get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(this->display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);

    // get an appropriate EGL frame buffer configuration
    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(EGL_FALSE != result);

    // create an EGL rendering context
    this->context = eglCreateContext(this->display, config, EGL_NO_CONTEXT, context_attributes);
    assert(this->context!=EGL_NO_CONTEXT);

    // create an EGL window surface
    success = graphics_get_display_size(0 /* LCD */, &this->width, &this->height);
    assert( success >= 0 );

    // set destination coordinates
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = this->width;
    dst_rect.height = this->height;

    // set source coordinates
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = this->width << 16;
    src_rect.height = this->height << 16;

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    dispman_element =
    vc_dispmanx_element_add(dispman_update, dispman_display,
                0/*layer*/, &dst_rect, 0/*src*/,
                &src_rect, DISPMANX_PROTECTION_NONE,
                0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

    this->nativewindow.element = dispman_element;
    this->nativewindow.width = this->width;
    this->nativewindow.height = this->height;
    vc_dispmanx_update_submit_sync( dispman_update );

    this->surface = eglCreateWindowSurface( this->display, config, &(this->nativewindow), NULL );
    assert(this->surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(this->display, this->surface, this->surface, this->context);
    assert(EGL_FALSE != result);
}
