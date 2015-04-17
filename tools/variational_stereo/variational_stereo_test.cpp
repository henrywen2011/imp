#include <assert.h>
#include <cstdint>
#include <iostream>
#include <memory>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <imp/core/roi.hpp>
#include <imp/core/image_raw.hpp>
#include <imp/core/image_cv.hpp>
#include <imp/cucore/cu_image_gpu.cuh>
#include <imp/cucore/cu_math.cuh>
#include <imp/bridge/opencv/cu_cv_bridge.hpp>

#include <imp/cudepth/variational_stereo.hpp>

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    imp::cu::ImageGpu32fC1::Ptr d_cones1_32fC1;
    imp::cu::cvBridgeLoad(d_cones1_32fC1, "/home/mwerlberger/data/std/cones/im2.ppm",
                          imp::PixelOrder::gray);
    imp::cu::ImageGpu32fC1::Ptr d_cones2_32fC1;
    imp::cu::cvBridgeLoad(d_cones2_32fC1, "/home/mwerlberger/data/std/cones/im6.ppm",
                          imp::PixelOrder::gray);

    {
      imp::Pixel32fC1 min_val,max_val;
      imp::cu::minMax(*d_cones1_32fC1, min_val, max_val);
      std::cout << "disp: min: " << min_val.x << " max: " << max_val.x << std::endl;
    }

    std::unique_ptr<imp::cu::VariationalStereo> stereo(
          new imp::cu::VariationalStereo());

    stereo->parameters()->verbose = 0;
    stereo->parameters()->solver = imp::cu::StereoPDSolver::PrecondHuberL1Weighted;
    stereo->parameters()->ctf.scale_factor = 0.8f;
    stereo->parameters()->ctf.iters = 50;
    stereo->parameters()->ctf.warps  = 10;
    stereo->parameters()->ctf.apply_median_filter = true;

    stereo->addImage(d_cones1_32fC1);
    stereo->addImage(d_cones2_32fC1);

    stereo->solve();

    std::shared_ptr<imp::cu::ImageGpu32fC1> d_disp = stereo->getDisparities();


    {
      imp::Pixel32fC1 min_val,max_val;
      imp::cu::minMax(*d_disp, min_val, max_val);
      std::cout << "disp: min: " << min_val.x << " max: " << max_val.x << std::endl;
    }

    imp::cu::cvBridgeShow("cones im2", *d_cones1_32fC1);
    imp::cu::cvBridgeShow("cones im6", *d_cones2_32fC1);
    *d_disp *= -1;
    {
      imp::Pixel32fC1 min_val,max_val;
      imp::cu::minMax(*d_disp, min_val, max_val);
      std::cout << "disp: min: " << min_val.x << " max: " << max_val.x << std::endl;
    }

    imp::cu::cvBridgeShow("disparities", *d_disp, true);
    cv::waitKey();
  }
  catch (std::exception& e)
  {
    std::cout << "[exception] " << e.what() << std::endl;
    assert(false);
  }

  return EXIT_SUCCESS;

}