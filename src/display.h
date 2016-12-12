#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <assert.h>

#include <bcm_host.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

class Display {
private:
    uint32_t width;
    uint32_t height;

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;

    EGL_DISPMANX_WINDOW_T nativewindow;

public:
    /**
     * @brief       get a reference to the camera object
     *
     * @return      reference to the camera object (singleton pattern)
     */
    static Display& get() {
        static Display display_instance;
        return display_instance;
    }

    inline uint32_t get_width() const {
        return this->width;
    }

    inline uint32_t get_height() const {
        return this->height;
    }

    inline float get_aspect_ratio() const {
        return (float)this->get_width() / (float)this->get_height();
    }

    inline EGLDisplay get_display() const {
        return this->display;
    }

    inline EGLDisplay get_surface() const {
        return this->surface;
    }

private:
    /**
     * @brief       camera constructor
     *
     * @return      camera instance
     */
    Display();

    void initialize_opengles();

    // singleton pattern
    Display(Display const&)          = delete;
    void operator=(Display const&)  = delete;
};

#endif //_DISPLAY_H
