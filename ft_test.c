#include <stdio.h>
#include <freetoup.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "meadecam.h"

#ifdef BUILD_MEADECAM
#include <meadecam.h>
#define FP(x) Toupcam_##x
#define CP(x) TOUPCAM_##x
#define XP(x) Toupcam##x
#define THAND HToupCam
#define DNAME "Toupcam"
#elif BUILD_FREETOUP
#define FP(x) FreeToup_##x
#define CP(x) FREETOUP_##x
#define XP(x) FreeToup##x
#define THAND HFreeToup
#define DNAME "FreeToup"
#endif

void hex_dump(void *data, size_t sz)
{
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < sz; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == sz) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == sz) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}

}

int EnumV2(int argc, char *argv[])
{
    int rc;

    XP(DeviceV2) pCameraInfo[CP(MAX)];

    rc = FP(EnumV2)(pCameraInfo);
    printf ("EnumV2 returned %d\n", rc);
    if (rc > 0) {
        int i;
        for (i = 0; i < rc; i++) {
            printf("hello\n");
            hex_dump(&pCameraInfo[i], sizeof(pCameraInfo[i]));
        }
        rc = 0;
    } else {
        rc = -1;
    }

    return rc;

}

int do_Open(int argc, char *argv[])
{
    int rc;
    THAND cam;

    XP(DeviceV2) pCameraInfo[CP(MAX)];

    rc = FP(EnumV2)(pCameraInfo);
    if (rc > 0) {
        cam = FP(Open)(pCameraInfo[0].id);
        if (!cam) {
            fprintf(stderr, "Open failed!\n");
        } else {
            printf("Got handle %p\n", cam);
            usleep(10000000);
            FP(Close)(cam);
        }
    } else {
        rc = -1;
    }

    return rc;

}

int do_Serial(int argc, char *argv[])
{
    int rc;
    THAND cam;

    XP(DeviceV2) pCameraInfo[CP(MAX)];

    rc = FP(EnumV2)(pCameraInfo);
    if (rc > 0) {
        cam = FP(Open)(pCameraInfo[0].id);
        if (!cam) {
            fprintf(stderr, "Open failed!\n");
        } else {
    // Get Firmware Info
    char firmwareBuffer[32] = {0};
    uint16_t pRevision = 0;
    FP(get_SerialNumber(cam, firmwareBuffer));
            printf("Serial: %s\n", firmwareBuffer);
            FP(Close)(cam);
        }
    } else {
        rc = -1;
    }

    return rc;

}

int do_FWVer(int argc, char *argv[])
{
    int rc;
    THAND cam;

    XP(DeviceV2) pCameraInfo[CP(MAX)];

    rc = FP(EnumV2)(pCameraInfo);
    if (rc > 0) {
        cam = FP(Open)(pCameraInfo[0].id);
        if (!cam) {
            fprintf(stderr, "Open failed!\n");
        } else {
    // Get Firmware Info
    char firmwareBuffer[32] = {0};
    uint16_t pRevision = 0;
    FP(get_FwVersion(cam, firmwareBuffer));
            printf("FWVer: %s\n", firmwareBuffer);
            FP(Close)(cam);
        }
    } else {
        rc = -1;
    }

    return rc;

}


int main(int argc, char *argv[])
{
    printf("hello\n");

    if (argc <= 1) {
        fprintf(stderr, "specify an operation!\n");
        return 1;
    }

    if (!strcmp(argv[1], "EnumV2")) {
        return EnumV2(argc - 1,  &argv[1]);           
    } else if (!strcmp(argv[1], "Open")) {
        return do_Open(argc - 1,  &argv[1]);           
    } else if (!strcmp(argv[1], "Serial")) {
        return do_Serial(argc - 1,  &argv[1]);           
    } else if (!strcmp(argv[1], "FWVer")) {
        return do_FWVer(argc - 1,  &argv[1]);           
    } else {
        fprintf(stderr, "Invalid selection %s\n", argv[1]);
        return 1;
    }

    return 0;
}
