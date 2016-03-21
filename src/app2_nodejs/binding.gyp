{
    "targets": [
    {
        "target_name": "addon",
            "sources": [ "addon.cc" ],
            "include_dirs": [
                "..",
            "<!(node -e \"require('nan')\")"
            ],
            "conditions": [
                ['OS=="linux"', {
                    'ldflags': [
                        '-L/home/w12x/src/wiki/build/src/understandinglib',
                        '-L/home/w12x/src/wiki/build/src/wikilib',
                        '-lunderstandinglib',
                    ],
                }]
            ]
    }
    ]
}
