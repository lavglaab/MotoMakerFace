
module.exports = [
  {
    "type": "heading",
    "defaultValue": "Moto Maker"
  },

  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Widgets"
      },

      {
        "type": "toggle",
        "messageKey": "enableWordmark",
        "defaultValue": "true",
        "label": "Show wordmark",
      },

      // {
      //   "type": "input",
      //   "messageKey": "textWordmark",
      //   "defaultValue": "pebble",
      //   "label": "Wordmark text",
      //   "attributes": {
      //     "placeholder": "eg. pebble",
      //     "limit": 25,
      //   },
      //   "description": "Text at top of dial",
      // },

      {
        "type": "toggle",
        "messageKey": "enableDouble12",
        "defaultValue": "true",
        "label": "Double mark at 12 o'clock position",
        },
    ]
  },

  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Dial colors"
      },

      {
      "type": "color",
      "messageKey": "colorBackground",
      "defaultValue": "000000",
      "label": "Background",
      "allowGray": "true",
      },

      {
        "type": "color",
        "messageKey": "colorHourHand",
        "defaultValue": "ffffff",
        "label": "Hour hand",
      },

      {
        "type": "color",
        "messageKey": "colorMinuteHand",
        "defaultValue": "ffffff",
        "label": "Minute hand",
      },

      {
        "type": "color",
        "messageKey": "colorSecondHand",
        "defaultValue": "ff0000",
        "label": "Second hand",
      },

      {
        "type": "color",
        "messageKey": "colorDot",
        "defaultValue": "ffffff",
        "label": "Dot",
        "description": "In the center of the watch face",
        "allowGray": "true",
      },

      {
        "type": "color",
        "messageKey": "colorHourMarkers",
        "defaultValue": "aaaaaa",
        "label": "Hour markers",
        "description": "12, 3, 6, and 9 o'clock markers",
      },

      {
        "type": "color",
        "messageKey": "colorMinuteMarkers",
        "defaultValue": "aaaaaa",
        "label": "Minute markers",
        "description": "Markers at every 5 minutes around the watch face",
      },

      {
        "type": "color",
        "messageKey": "colorWordmark",
        "defaultValue": "aaaaaa",
        "label": "Wordmark at top of dial",
        "allowGray": false,
      },
    ]
  },

  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Features"
      },

      {
      "type": "toggle",
      "messageKey": "enableSecondHand",
      "defaultValue": "true",
      "label": "Show second hand",
      "description": "Uses more battery"
      },

      {
        "type": "toggle",
        "messageKey": "enableVibrateOnDisconnect",
        "defaultValue": "true",
        "label": "Vibrate on disconnect",
        },
    ]
  },

  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
