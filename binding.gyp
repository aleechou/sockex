{
  "targets": [
    {
      "target_name": "sockex",
      "sources": [ "sockex.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
