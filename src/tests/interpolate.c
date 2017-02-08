#include <string.h>
#include <float.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../hrtf/mysofa.h"
#include "../hrtf/tools.h"
#include "tests.h"

void test_interpolate() {
	struct MYSOFA_HRTF *hrtf = NULL;
	int err = 0;
	int i;

	hrtf = mysofa_load("tests/sofa_api_mo_test/MIT_KEMAR_normal_pinna.sofa",
				&err);

	if (!hrtf) {
		CU_FAIL_FATAL("Error reading file.");
	}

	mysofa_tocartesian(hrtf);

	float fir[hrtf->N * hrtf->R];
	float delays[2];
	float *res;
	int neighborhood[6] = { -1, -1, -1, -1, -1, -1 };

	res = mysofa_interpolate(hrtf, hrtf->SourcePosition.values, 0, neighborhood,
			fir, delays);
	CU_ASSERT(res == hrtf->DataIR.values);
	CU_ASSERT(delays[0] == 0);
	CU_ASSERT(delays[1] == 0);

	float c[3];
	c[0] = (hrtf->SourcePosition.values[0] + hrtf->SourcePosition.values[3])
			/ 2;
	c[1] = (hrtf->SourcePosition.values[1] + hrtf->SourcePosition.values[4])
			/ 2;
	c[2] = (hrtf->SourcePosition.values[2] + hrtf->SourcePosition.values[5])
			/ 2;
	neighborhood[0] = 1;

	res = mysofa_interpolate(hrtf, c, 0, neighborhood, fir, delays);
	CU_ASSERT(res == fir);
	CU_ASSERT(delays[0] == 0);
	CU_ASSERT(delays[1] == 0);

	for (i = 0; i < hrtf->N * hrtf->R; i++) {
#ifdef VDEBUG
		printf("%f %f %f\n", res[i], hrtf->DataIR.values[i],
				hrtf->DataIR.values[i + hrtf->N * hrtf->R]);
#endif
		CU_ASSERT(fequals(res[i],(hrtf->DataIR.values[i]+hrtf->DataIR.values[i+hrtf->N*hrtf->R])/2));
	}

	// TODO add some tests...

	mysofa_free(hrtf);
}