#include "StudentService.Client.h"
#include "Win32EmbedXrpcPort.h"

void BroadcastDataTimeClientImpl::BroadcastDataTime(DateTime_t t)
{
    printf("client %4d-%2d-%2d  %2d:%2d:%2d\n", t.Year, t.Month, t.Day, t.Hour, t.Min, t.Sec);
}