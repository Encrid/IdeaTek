/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_audio.h>

int main(void)
{
    ideatek_audio_request_t request = {
        .direction = IDEATEK_AUDIO_DIRECTION_RECORDING };

    if (request.channels != 0)
        return (1);
    return (sizeof(ideatek_audio_state_t_desc) > 1 ? 0 : 1);
}
