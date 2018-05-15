// https://blog.csdn.net/lvbian/article/details/19842719
// http://ftp.gnome.org/pub/gnome/sources/gnome-screenshot/3.22/

#include <gtk/gtk.h>

int main(int argc,char **argv)
{   
    gtk_init(&argc,&argv);
    GdkWindow *window;
    GError *error = NULL;
    // 全屏
    window = gdk_get_default_root_window();
    // 活动窗口
    //window = gdk_screen_get_active_window (screen);
    GdkScreen *screen = gdk_screen_get_default();    
    gint w = gdk_screen_get_width(screen);
    gint h = gdk_screen_get_height(screen);    
    //g_message ("%d X %d", w, h);
    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_window (window, 0, 0, w, h);

    // 鼠标转图片
    GdkCursor *cursor;
    GdkPixbuf *pixbuf_cursor;
    cursor = gdk_cursor_new_for_display (gdk_display_get_default (), GDK_LEFT_PTR);
    pixbuf_cursor = gdk_cursor_get_image (cursor);
    // 在这里保存鼠标图片会成功，但是后面代码全失效，为什么呢？
    //gdk_pixbuf_save(pixbuf_cursor, "mouse.png", "png", &error, NULL);
    //g_message ("save mouse pixbuf: %s", error->message);    
    GdkRectangle rect;
    rect.width = gdk_pixbuf_get_width (pixbuf_cursor);
    rect.height = gdk_pixbuf_get_height (pixbuf_cursor);
    
    // 获取鼠标坐标
    GdkDeviceManager *manager;
    GdkDevice *device;
    manager = gdk_display_get_device_manager (gdk_display_get_default());
    device = gdk_device_manager_get_client_pointer (manager);
    gint xc, yc;
    gdk_window_get_device_position (window, device, &xc, &yc, NULL);
    g_message ("mouse position: %d , %d", xc, yc);

    // 合成图片。offset 和 dest 坐标相同，为什么？
    gdk_pixbuf_composite (pixbuf_cursor, pixbuf, xc, yc, rect.width, rect.height, xc, yc, 1.0, 1.0, GDK_INTERP_BILINEAR, 255);    

    // 保存图片
    GDateTime *d;
    d = g_date_time_new_now_local ();
    char *cd = g_date_time_format (d, "%Y%m%d%H%M%S");
    char filename[20];
    sprintf(filename, "%s.jpg", cd);
    //g_message ("filename: %s", filename);
    gdk_pixbuf_save(pixbuf, filename, "jpeg", &error, "quality", "100", NULL);    
    g_message ("save screenshot: %s", error->message);

    gtk_main();
    return FALSE;
}
