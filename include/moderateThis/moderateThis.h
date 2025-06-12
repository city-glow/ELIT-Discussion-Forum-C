#ifndef MODERATETHIS_H
#define MODERATETHIS_H

#include <stdbool.h>
#include "../id/id.h"

typedef struct {
    Id request_id;        // ID dari permintaan
    Id user_id;           // User yang meminta izin posting
    Id board_id;          // Board yang diminta
    bool is_approved;     // Status moderasi (false default, true jika disetujui)
} ModerateRequest;

void create_moderate_request(ModerateRequest *request, Id request_id, Id user_id, Id board_id);

#endif
