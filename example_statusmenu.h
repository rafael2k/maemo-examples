#ifndef EXAMPLE_STATUS_MENU_ITEM_H
#define EXAMPLE_STATUS_MENU_ITEM_H

#include <libhildondesktop/libhildondesktop.h>

G_BEGIN_DECLS

#define EXAMPLE_TYPE_STATUS_MENU_ITEM            (example_status_menu_item_get_type ())
#define EXAMPLE_STATUS_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), EXAMPLE_TYPE_STATUS_MENU_ITEM, ExampleStatusMenuItem))
#define EXAMPLE_STATUS_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), EXAMPLE_TYPE_STATUS_MENU_ITEM, ExampleStatusMenuItemClass))
#define EXAMPLE_IS_STATUS_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EXAMPLE_TYPE_STATUS_MENU_ITEM))
#define EXAMPLE_IS_STATUS_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), EXAMPLE_TYPE_STATUS_MENU_ITEM))
#define EXAMPLE_STATUS_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), EXAMPLE_TYPE_STATUS_MENU_ITEM, ExampleStatusMenuItemClass))

typedef struct _ExampleStatusMenuItem        ExampleStatusMenuItem;
typedef struct _ExampleStatusMenuItemClass   ExampleStatusMenuItemClass;
typedef struct _ExampleStatusMenuItemPrivate ExampleStatusMenuItemPrivate;

struct _ExampleStatusMenuItem
{
  HDStatusMenuItem       parent;

  ExampleStatusMenuItemPrivate       *priv;
};

struct _ExampleStatusMenuItemClass
{
  HDStatusMenuItemClass  parent;
};

GType example_status_menu_item_get_type (void);

G_END_DECLS

#endif

