// handle of connection to main page
var myConnectionBg;
// prefix of every logs
var prefixBg = "background.js: \n\t";
// prefix of txtState
var prefixTs = "BT Connection State: <br />&emsp;";
// cuurrent socket ID
var curSocketId;
// smartcar BT information
var mySmartCarBtInfo;
// store the incomplete byte array
var incompleteBytes = [];
// password
var passWord = "fish";
//
var passWordChecked = false;
var isPlotting = false;
var isConnecting = false;
var isRetrying = false;
var allowReceive = false;
//
var hasWindow = false;
//
var windowSizeRatio = 0.5818815;
//
var channelString = "int32_t";
var typeNameArray = ["int8_t", "int16_t", "int32_t", "float"];

//
var hWnd;
var hAppWindow;
//
var hTextState;
var hTextLength;
var hLineChart;
var hButtonConnect;
var hButtonPlot;
var hCheckboxAutoconnect;
var hTextboxBtdevicename;
var hLoadingIcon;
var hScaleMin;
var hScaleMax;
var hScaleStep;
var hChannelOption;
//
var hTimer;
var hLoadingTimer;

//
var tempFloat;

var isProcessing = false;

// check if the variable is defined
var isDefined =
function(v)
{
  return (typeof v !== "undefined");
};

// print error msg
var printLastError =
function(txt)
{
  try
  {
    console.assert(!isDefined(chrome.runtime.lastError.string), prefixBg + txt + chrome.runtime.lastError.string);
    return false;
  }
  catch (e)
  {
    if (e.message == "Cannot read property 'string' of undefined")
      return true;
    else
      return false;
  }
};

var clearProc = 
function(sockets)
{
  for (socket of sockets)
  {
    chrome.bluetoothSocket.disconnect(socket.socketId, generalErrorHandler);
    chrome.bluetoothSocket.close(socket.socketId, generalErrorHandler);
  }
};

var closeAllSockets = 
function()
{
  chrome.bluetoothSocket.getSockets(clearProc);
};

var socketRetry =
function(errorStr)
{
  hTextState.innerHTML = prefixTs + errorStr;
  isConnecting = false;
  isPlotting = false;
  allowReceive = false;
  hButtonConnect.value = "Connect";
  hButtonConnect.removeAttribute("disabled");
  hTextboxBtdevicename.removeAttribute("disabled");
  hButtonPlot.setAttribute("disabled", "");
  hLoadingIcon.setAttribute("hidden", "");
  hScaleMin.removeAttribute("disabled");
  hScaleMax.removeAttribute("disabled");
  hScaleStep.removeAttribute("disabled");
  hButtonPlot.value = "Start";
  chrome.bluetoothSocket.setPaused(curSocketId, false, generalErrorHandler);
  incompleteBytes = [];
  closeAllSockets();
  if (isDefined(hTimer))
  {
    if (isDefined(curSocketId))
    {
      chrome.bluetoothSocket.send(curSocketId, toArrayBuffer('e'), checkConnection);
    }
    clearInterval(hTimer);
  }
  
  if (hCheckboxAutoconnect.checked)
  {
    isRetrying = true;
    isConnecting = true;
    hButtonConnect.setAttribute("disabled", "");
    hTextboxBtdevicename.setAttribute("disabled", "");  
    hLoadingIcon.removeAttribute("hidden");
    hTextState.innerHTML = prefixTs + "Retry......";
    chrome.bluetooth.getAdapterState(connectSmartCar);
  }
  else if (isDefined(curSocketId))
  {
    isRetrying = false;
  }
};

var checkConnection = 
function(n)
{
  if (n == 0)
    socketRetry("Send Data Error...");
};

var generalErrorHandler =
function()
{
  try
  {
    chrome.runtime.lastError.string;
  }
  catch (e) {}
};

var toArrayBuffer = 
function(ch)
{
  var buf = new ArrayBuffer(2);
  var arr = new Uint8Array(buf);
  arr[0] = ch.charCodeAt();
  arr[1] = 0;
  return buf;
};

var charCodetoArrayBuffer = 
function(cc)
{
  var buf = new ArrayBuffer(2);
  var arr = new Uint8Array(buf);
  arr[0] = cc;
  arr[1] = 0;
  return buf;
};

var graphReset = 
function()
{
  if (isDefined(hLineChart))
    hLineChart.destroy();

  var lineChartData = 
    {
      labels : [""],
      datasets :
      [
        {
        	label: "My First dataset",
        	fillColor : "rgba(255,0,0,0)",
        	strokeColor : "rgba(255,0,0,1)",
        	pointColor : "rgba(255,0,0,1)",
        	pointStrokeColor : "#fff",
        	pointHighlightFill : "#fff",
        	pointHighlightStroke : "rgba(255,0,0,1)",
        	data : [0]
        },
        {
        	label: "My Second dataset",
        	fillColor : "rgba(0,255,0,0",
        	strokeColor : "rgba(0,255,0,1)",
        	pointColor : "rgba(0,255,0,1)",
        	pointStrokeColor : "#fff",
        	pointHighlightFill : "#fff",
        	pointHighlightStroke : "rgba(0,255,0,1)",
        	data : [0]
        },
        {
        	label: "My third dataset",
        	fillColor : "rgba(0,0,255,0)",
        	strokeColor : "rgba(0,0,255,1)",
        	pointColor : "rgba(0,0,255,1)",
        	pointStrokeColor : "#fff",
        	pointHighlightFill : "#fff",
        	pointHighlightStroke : "rgba(0,0,255,1)",
        	data : [0]
        }
      ]
    };
  
	hLineChart = new Chart(hWnd.document.getElementById("canvas").getContext("2d"))
	                      .Line(lineChartData,
                        {
                          responsive:	true,
                          animation:	false,
                          scaleOverride: true,
                          scaleSteps: parseInt(hScaleStep.value),
                          scaleStepWidth: parseFloat((hScaleMax.value - hScaleMin.value) / hScaleStep.value),
                          scaleStartValue: parseFloat(hScaleMin.value),
                          scaleIntegersOnly: false,
                          scaleShowVerticalLines: false,
                          maintainAspectRatio: true,
                        });
  
	hLineChart.reflow();

	for (var i = 0; i < 50; i++)
		hLineChart.addDatasWithoutUpdate([0, 0, 0], "");
  
	hLineChart.update();
};

var DataConverter = 
{
  dataUpdate:
  function()
  {
    if (incompleteBytes.length <= this.byteCount * 50 && incompleteBytes.length)
    {
      isProcessing = true;
      
      var l = Math.floor(incompleteBytes.length / this.byteCount);
      for (var i = 0; i < l; i++)
        this.plotData(i);
      
      hLineChart.update();
      //hTextLength.innerHTML = "Data Received: " + incompleteBytes.length + " B";
      isProcessing = false;
    }
    else if (incompleteBytes.length > this.byteCount * 50)
    {
      isProcessing = true;
      
      for (var i = incompleteBytes.length - this.byteCount * 50; i < incompleteBytes.length; i += this.byteCount)
        this.plotData(i);
      incompleteBytes = [];
        
      hLineChart.update();
      //hTextLength.innerHTML = "Data Received: " + incompleteBytes.length + " B";
      isProcessing = false;
    }
  },
  plotData:
  function(bytesStartIndex)
  {
    hLineChart.addAndRemoveDatasWithoutUpdate(this.getDataList());
  },
  getDataList:
  function(bytesStartIndex)
  {
    var retList = [];
    for (var i = 0; i < this.typeIndex.length; i++)
      retList.push(this.dispatch(i, bytesStartIndex));
    return retList;
  },
  dispatch:
  function(i, bytesStartIndex)
  {
    switch (this.typeIndex[i])
    {
      case 0:
        return (new Int8Array(this.getBluetoothData(bytesStartIndex, 1)))[0];
        break;
      case 1:
        return (new Int16Array(this.getBluetoothData(bytesStartIndex, 2)))[0];
        break;
      case 2:
        return (new Int32Array(this.getBluetoothData(bytesStartIndex, 4)))[0];
        break;
      case 3:
        return (new Float32Array(this.getBluetoothData(bytesStartIndex, 4)))[0];
        break;
    }
  },
  getBluetoothData:
  function(index, size)
  {
    return (new Int8Array(incompleteBytes.splice(index, size))).buffer;
  },
  typeIndex: [],
  byteCount: 0
};

var onDataUpdate =
function()
{
  if (!isProcessing && isPlotting)
    DataConverter.dataUpdate();
};

var bluetoothOnReceive =
function(info)
{
  if (isPlotting)
  {
    if (!allowReceive)
    {
      if ((new Int8Array(info.data))[0] != '/'.charCodeAt())
        return ;
      allowReceive = true;
      return ;
    }
    for (var byte of (new Int8Array(info.data)))
      incompleteBytes.push(byte);
  }
};

var bluetoothOnReceiveError =
function(info)
{
  console.log(prefixBg + "OnReceiveError" + info.errorMessage);
  socketRetry("Disconnected...");
};

var bluetoothSocketOnConnected =
function()
{
  if (printLastError("Connect Error : "))
  {
    hTextState.innerHTML = prefixTs + "{ Connected }";
    hButtonConnect.removeAttribute("disabled");
    hButtonConnect.value = "Disconnect";
    isConnecting = false;
    isRetrying = false;
    allowReceive = false;
    hButtonPlot.removeAttribute("disabled");
    hLoadingIcon.setAttribute("hidden", "");
    chrome.bluetoothSocket.send(curSocketId, toArrayBuffer('e'), checkConnection);
  }
  else
    socketRetry("Connection error...");
};

// connect to SmartCar after socket created
var bluetoothSocketOnCreated =
function(createInfo)
{
  curSocketId = createInfo.socketId;
  try
  {
    // try to connect to SmartCar
    chrome.bluetoothSocket.connect(curSocketId, mySmartCarBtInfo.address, mySmartCarBtInfo.uuids[0], bluetoothSocketOnConnected);
  }
  catch (e)
  {
    socketRetry("Error: 404 - Car not found!");
  }
};

var switchPlotting =
function()
{
  if (hButtonPlot.value == "Start")
  {
    hButtonPlot.value = "Stop";
    hScaleMin.setAttribute("disabled", "");
    hScaleMax.setAttribute("disabled", "");
    hScaleStep.setAttribute("disabled", "");
    graphReset();
    isPlotting = true;
    allowReceive = false;
    incompleteBytes = [];
    
    chrome.bluetoothSocket.send(curSocketId, toArrayBuffer('s'), checkConnection);
    hTimer = setInterval(onDataUpdate, 10);
  }
  else if (hButtonPlot.value == "Stop")
  {
    if (isDefined(hTimer))
      clearInterval(hTimer);
      
    chrome.bluetoothSocket.send(curSocketId, toArrayBuffer('e'), checkConnection);
    
    hButtonPlot.value = "Start";
    hScaleMin.removeAttribute("disabled");
    hScaleMax.removeAttribute("disabled");
    hScaleStep.removeAttribute("disabled");
    isPlotting = false;
    allowReceive = false;
    incompleteBytes = [];
  }
};

var connectSmartCar =
function(adapterInfo)
{
  //if (adapterInfo)
  if (!printLastError("BT Adapter Error: "))
  {
    socketRetry("BT Adapter Error");
    return;
  }
  else if(!adapterInfo.powered || !adapterInfo.available)
  {
    socketRetry("Bluetooth not available!");
    return;
  }
  
  // start discovering BT devices
  chrome.bluetooth.startDiscovery(generalErrorHandler);
  
  // stop discovering BT devices after 4 seconds
  setTimeout
  (
    function()
    {
      // get BT device list
      chrome.bluetooth.getDevices
      (
        function(deviceInfos)
        {
          // find SmartCar in the list
          for (var device of deviceInfos)
          {
            if (device.name == hTextboxBtdevicename.value)
            {
              // send SmartCar BT info to main page
              if (!isRetrying)
                hTextState.innerHTML = prefixTs + "SmartCar Paired... Connecting...";
              // TODO: show smartcar's info
              mySmartCarBtInfo = device;
              
              // create bluetooth socket
              chrome.bluetoothSocket.create(bluetoothSocketOnCreated);
              
              return ;
            }
          }
          hTextState.innerHTML = prefixTs + "Device Name not available!";
        }
      );
    },
    500
  );
};

var onButtonClick = 
function(event)
{
  var button = event.target;
  
  switch (button.value)
  {
    case "Connect":
      if (!isConnecting)
      {
        closeAllSockets();
        isConnecting = true;
        hButtonConnect.setAttribute("disabled", "");
        hTextboxBtdevicename.setAttribute("disabled", "");
        hLoadingIcon.removeAttribute("hidden");
        chrome.bluetooth.getAdapterState(connectSmartCar);
      }
      break;
    case "Disconnect":
      socketRetry("Disconnected...");
      break;
    case "Start":
    case "Stop":
      switchPlotting();
      break;
  }
};

var onTextBoxFocus = 
function(event)
{
  if (event.target.id == "btDeviceName")
  {
    // start discovering BT devices
    chrome.bluetooth.startDiscovery(generalErrorHandler);
    
    // stop discovering BT devices after 4 seconds
    hGetDeviceTimer = setTimeout
    (
      function()
      {
        // get BT device list
        chrome.bluetooth.getDevices
        (
          function(deviceInfos)
          {
            for (; hTextboxBtdevicename.childNodes.length > 0;)
              hTextboxBtdevicename.removeChild(hTextboxBtdevicename.childNodes[0]);
            for (var device of deviceInfos)
            {
              var opt = document.createElement("option");
              opt.value = device.name;
              opt.innerText = device.name;
              hTextboxBtdevicename.appendChild(opt);
            }
          }
        );
      },
      250
    );
  }
};

var onScaleTextBoxFocusOut = 
function(event)
{
  if (parseFloat(event.target.value) <= parseFloat(event.target.max) && parseFloat(event.target.value) >= parseFloat(event.target.min))
  {
    graphReset();
    chrome.storage.sync.set({'scaleMin': parseFloat(hScaleMin.value), 'scaleMax': parseFloat(hScaleMax.value), 'scaleStep': parseInt(hScaleStep.value), 'hasData': true});
  }
  else
    event.target.value = event.target.defaultValue;
};

var onChannelOptionChanged = 
function(event)
{
  // if (event.target.value.length == 0)
  //   return ;
  DataConverter.typeIndex = [];
  DataConverter.byteCount = 0;
  var typeList = (event.target.value.replace(/\s/g, "")).toLowerCase().split(",");
  for (var type of typeList)
  {
    switch (type)
    {
      case "int8":
        DataConverter.typeIndex.push(0);
        DataConverter.byteCount += 1;
        break;
      case "int16_t":
        DataConverter.typeIndex.push(1);
        DataConverter.byteCount += 2;
        break;
      case "int32_t":
        DataConverter.typeIndex.push(2);
        DataConverter.byteCount += 4;
        break;
      case "float":
        DataConverter.typeIndex.push(3);
        DataConverter.byteCount += 4;
        break;
      default:
        event.target.value = channelString;
        return ;
    }
  }
  channelString = event.target.value;
  chrome.storage.sync.set({'shownTypes': channelString, 'hasData': true});
};

// var onDeviceChangedHandler = 
// function(device)
// {
//   if (isDefined(mySmartCarBtInfo))
//   {
//     if (device.name == mySmartCarBtInfo.name)
//       mySmartCarBtInfo = device;
//   }
// };

var onClosedHandler = 
function()
{
  closeAllSockets();
  if (isDefined(hTimer))
    clearInterval(hTimer);
  if (isDefined(hTimer))
  {
    chrome.bluetoothSocket.send(curSocketId, toArrayBuffer('e'), checkConnection);
    clearInterval(hTimer);
  }
  hasWindow = false;
};

// prepare for connection to other pages
var connectPage =
function(createdWindow)
{
  hWnd = createdWindow.contentWindow;
  hAppWindow = createdWindow;
  
  hWnd.addEventListener
  (
    "load",
    function()
    {
      hTextState = hWnd.document.getElementById("textState");
      hTextLength = hWnd.document.getElementById("textLength");
      
      hButtonConnect = hWnd.document.getElementById("connect");
      hButtonPlot = hWnd.document.getElementById("plotting");
      hCheckboxAutoconnect = hWnd.document.getElementById("autoConnection");
      hTextboxBtdevicename = hWnd.document.getElementById("btDeviceName");
      hLoadingIcon = hWnd.document.getElementById("loading");
      
      hScaleMin = hWnd.document.getElementById("min");
      hScaleMax = hWnd.document.getElementById("max");
      hScaleStep = hWnd.document.getElementById("step");
      hChannelOption = hWnd.document.getElementById("channelType");
      
      hButtonConnect.addEventListener("click", onButtonClick);
      hButtonPlot.addEventListener("click", onButtonClick);
      hTextboxBtdevicename.addEventListener("focus", onTextBoxFocus);
      hScaleMin.addEventListener("focusout", onScaleTextBoxFocusOut);
      hScaleMax.addEventListener("focusout", onScaleTextBoxFocusOut);
      hScaleStep.addEventListener("focusout", onScaleTextBoxFocusOut);
      hChannelOption.addEventListener("focusout", onChannelOptionChanged);
      hWnd.document.getElementById("control").addEventListener
      (
        "keypress",
        function(event)
        {
          chrome.bluetoothSocket.send(curSocketId, charCodetoArrayBuffer(event.keyCode), checkConnection);
          this.value = "";
          return true;
        }
      );
      
      chrome.storage.sync.get
      (
        ['hasData', 'scaleMin', 'scaleMax', 'scaleStep', 'shownTypes'],
        function(items)
        {
          if (items.hasData)
          {
            hScaleMin.value = items.scaleMin;
            hScaleMax.value = items.scaleMax;
            hScaleStep.value = items.scaleStep;
            channelString = items.shownTypes;
            hChannelOption.value = channelString;
            onChannelOptionChanged({target: hChannelOption});
          }
          
          graphReset();
        }
      );
      
      // get BT device list
      chrome.bluetooth.getDevices
      (
        function(deviceInfos)
        {
          for (var device of deviceInfos)
          {
            var opt = document.createElement("option");
            opt.value = device.name;
            opt.innerText = device.name;
            hTextboxBtdevicename.appendChild(opt);
          }
        }
      );
      
      hTextState.innerHTML = prefixTs + "Ready...";
    },
    true
  );
  
  // chrome.bluetooth.onDeviceChanged.addListener(onDeviceChangedHandler);
  
  createdWindow.onClosed.addListener(onClosedHandler);
  createdWindow.onBoundsChanged.addListener(onResizedHandler);
};

var onResizedHandler = 
function()
{
  if (Math.abs(hAppWindow.outerBounds.height / hAppWindow.outerBounds.width - windowSizeRatio) > 0.005 && !hAppWindow.isMaximized())
    hAppWindow.outerBounds.setSize(Math.round(hAppWindow.outerBounds.width), Math.round(hAppWindow.outerBounds.width * windowSizeRatio));
  hLineChart.resize();
  hLineChart.reflow();
  hLineChart.update();
};

// TODO:  Use "onBoundsChanged" to keep window size ratio
//        Show color meaning
//        Random color
//        Set the y-axis range of the graph
//
// WISH:  Control/Start the car here

// create appWindow when the extension lauched
var onLaunched =
function()
{
  if (!hasWindow)
  {
    chrome.app.window.create
    (
      'page/main.html',
      {
        id: "main",
        outerBounds:
        {
          'width': 1420,
          'height': 830,
          'minWidth': 1420,
          'minHeight': 830,
          'left': 100,
          'top': 100
        }
        //'alwaysOnTop': true
      },
      connectPage
    );
    
    hasWindow = true;
  }
  return true;
};

// background.js starts here
chrome.app.runtime.onLaunched.addListener(onLaunched);
chrome.bluetoothSocket.onReceive.addListener(bluetoothOnReceive);
chrome.bluetoothSocket.onReceiveError.addListener(bluetoothOnReceiveError);
