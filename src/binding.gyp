{
  "targets": [
    {
      "target_name": "vdmx",
      "sources": [ "velleman-node-dmx.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
