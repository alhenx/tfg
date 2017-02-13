(function (window, $, undefined) {
  'use strict';
  var prueba;
  var DISABLED = "DISABLED";
  var ruedaGrados = 0;
  var automotor = 0;
  var tiempo = 0;
  var sensorYaw;
  var ledaux = 0;
  prueba = function prueba(cockpit) {
    console.log('Loading prueba plugin in the browser.');
    var rov = this;

    rov.cockpit = cockpit;
    rov.light = 0;

    crearRueda();

    $("#confirmar").click(function(){
      cerrar(0);
    });
    
    $("#cancelar").click(function(){
      cerrar(1);
    });
    this.cockpit.socket.on('navdata', function (data) {
      if (!jQuery.isEmptyObject(data)) {
        requestAnimationFrame(function () {
          sensorYaw=data.hdgd;
        });
      }
    });


    rov.cockpit.emit('inputController.register',
      [
        {
          name: "prueba.leds",
          description: "On/Off LEDs",
          defaults: { keyboard: '9' },
          down: function () {
          	if (ledaux==0){
         		rov.ledsaux(0);
         		ledaux=1;
          	}
         	else{
         		rov.ledsaux(1);
         		ledaux=0;
         	}
          }
        },
        {
          name: "prueba.auto",
          description: "Recta automática",
          defaults: { keyboard: '7' },
          down: function () {
            if (automotor==0){
              rov.cockpit.emit('rovpilot.setThrottle', 1);
              automotor=1;
            }else{
              rov.cockpit.emit('rovpilot.setThrottle', 0);
              automotor=0;
            }
          }
        },
        {
          name: "prueba.grados",
          description: "Cambia el valor de los grados",
          defaults: { keyboard: '6' },
          down: function () {
            rov.autogiro();
          }
        },
        {
          name: "prueba.prueba2",
          description: "prueba.",
          defaults: { keyboard: '0' },
          down: function () {
            guardarVal();
          }
        }
      ]);
  };

  prueba.prototype.ledsaux = function ledsaux(valor) {
    this.cockpit.socket.emit('cmd_update', {cmd:"ledaux",val:valor});
  };
  prueba.prototype.autogiro = function autogiro() {
    this.cockpit.socket.emit('cmd_update', {cmd:"holdHeading_toggle",val:ruedaGrados});
  };
  prueba.prototype.pruebagiro = function pruebagiro(valor) {
    this.cockpit.socket.emit('cmd_update', {cmd:"holdHeading_toggle",val:valor});
  };

  function crearRueda(){
    document.getElementById("ruedavalor").value=ruedaGrados;
    var wheel = new wheelnav("wheelDiv");
    wheel.slicePathFunction = slicePath().StarSlice;
    wheel.markerPathFunction = markerPath().PieLineMarker;
    wheel.clickModeRotate = false;
    wheel.wheelRadius = 75;
    wheel.markerEnable = false;
    wheel.slicePathAttr = { stroke: '#000000', 'stroke-width': 2 };
    wheel.linePathAttr = { stroke: '#000000', 'stroke-width': 2 };
    wheel.titleAttr = { fill: '#000000', stroke: 'none' };
    wheel.lineHoverAttr = { stroke: '#000000', 'stroke-width': 2 };
    wheel.titleHoverAttr = { fill: '#000000', stroke: 'none' };
    wheel.sliceSelectedAttr = { stroke: '#FFFFFF', 'stroke-width': 3 };
    wheel.lineSelectedAttr = { stroke: '#FFFFFF', 'stroke-width': 3 };
    wheel.titleSelectedAttr = { fill: '#FFFFFF' };

    wheel.createWheel(["90","135","180","-135","-90","-45","0","45"]);
    wheel.navItems[0].navigateFunction = function () { ruedaActu(90);};
    wheel.navItems[1].navigateFunction = function () { ruedaActu(135); };
    wheel.navItems[2].navigateFunction = function () { ruedaActu(180); };
    wheel.navItems[3].navigateFunction = function () { ruedaActu(-135); };
    wheel.navItems[4].navigateFunction = function () { ruedaActu(-90); };
    wheel.navItems[5].navigateFunction = function () { ruedaActu(-45); };
    wheel.navItems[6].navigateFunction = function () { ruedaActu(0); };
    wheel.navItems[7].navigateFunction = function () { ruedaActu(44); };
  }

  function ruedaActu(grado){
    document.getElementById("ruedavalor").value=grado;
  }

  function guardarVal(){
  	alert(sensorYaw);
  }
  window.Cockpit.plugins.push(prueba);
}(window, jQuery));
