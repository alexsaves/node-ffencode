{
  "targets": [{
    "target_name": "ffencode",
    "include_dirs" : [
      "src",
      "<!(node -e \"require('nan')\")"
    ],
    "sources": [
      "src/utils.cc",
      "src/index.cc",
      "src/lodepng.cc",
      "src/FFEncoder.cc",
      "src/cencode.cc",
      "src/jpeg/bit_writer.cc",
      "src/jpeg/colors_rgb.cc",
      "src/jpeg/dichotomy.cc",
      "src/jpeg/enc.cc",
      "src/jpeg/fdct.cc",
      "src/jpeg/headers.cc",
      "src/jpeg/jpeg_tools.cc",
      "src/jpeg/score_7.cc",
      "src/jpeg/yuv_convert.cc"
    ]
  }]
}