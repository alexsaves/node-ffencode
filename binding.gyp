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
      "src/FFEncoder.cc"
    ]
  }]
}