#ifndef NOTIFLIST_H
#define NOTIFLIST_H
#include "notif.h"

typedef struct NotifTElmtList *NotifAddress;
typedef struct NotifTElmtList {
  Notif info;
  NotifAddress nextNotif;
} NotifElmtList;
typedef NotifAddress NotifList;

#endif
