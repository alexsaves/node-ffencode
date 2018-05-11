{
  "targets": [
    {
      "target_name": "ffencode",
      "sources": [
        "cpp/ffencode.cpp"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}