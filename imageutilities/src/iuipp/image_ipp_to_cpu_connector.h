/*
 * Copyright (c) ICG. All rights reserved.
 *
 * Institute for Computer Graphics and Vision
 * Graz University of Technology / Austria
 *
 *
 * This software is distributed WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the above copyright notices for more information.
 *
 *
 * Project     : ImageUtilities
 * Module      : IPP-to-CPU Connector
 * Class       : none
 * Language    : C
 * Description : Definition of some memory conversions so that an ImageIPP can be used directly instead of an ImageCPU
 *
 * Author     : Manuel Werlberger
 * EMail      : werlberger@icg.tugraz.at
 *
 */

#ifndef IMAGE_IPP_TO_CPU_CONNECTOR_H
#define IMAGE_IPP_TO_CPU_CONNECTOR_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the IU API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//

#include <iudefs.h>
//#include <iucore.h>
#include "memorydefs_ipp.h"

namespace iu {

/** Converts the ImageIPP structure to an ImageCPU type. The data keeps owned with the src image.
 * @param[in] src Source image which still owns the data buffer after the conversion.
 * @returns ImageCpu type of the corresponding memory. No data owned from this instance.
 * @attention The returned image structured only holds a pointer to the original data structure.
 */
iu::ImageCpu_32f_C1* convertToCpu(iu::ImageIpp_32f_C1* src)
{
  iu::ImageCpu_32f_C1* img = new iu::ImageCpu_32f_C1(src->data(), src->width(), src->height(),
                                                     src->pitch(), true);
  return img;
}

/** Converts the ImageIPP structure to an ImageCPU type. The data keeps owned with the src image.
 * @param[in] src Source image which still owns the data buffer after the conversion.
 * @returns ImageCpu type of the corresponding memory. No data owned from this instance.
 * @attention The returned image structured only holds a pointer to the original data structure.
 */
iu::ImageCpu_32f_C3* convertToCpu(iu::ImageIpp_32f_C3* src)
{
  iu::ImageCpu_32f_C3* img = new iu::ImageCpu_32f_C3((float3*)src->data(), src->width(), src->height(),
                                                     src->pitch(), true);
  return img;
}



} // namespace iuprivate

#endif // IMAGE_IPP_TO_CPU_CONNECTOR_H
