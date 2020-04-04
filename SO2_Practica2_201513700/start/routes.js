'use strict'

/*
|--------------------------------------------------------------------------
| Routes
|--------------------------------------------------------------------------
|
| Http routes are entry points to your web application. You can create
| routes for different URL's and bind Controller actions to them.
|
| A complete guide on routing is available here.
| http://adonisjs.com/docs/4.1/routing
|
*/

/** @type {typeof import('@adonisjs/framework/src/Route/Manager')} */
const Route = use('Route')
var http = require('http').Server()
var io = require('socket.io')(http)
var osu = require('node-os-utils')
var cpu = osu.cpu
var os = require('os-utils')

function ObtenerTextoArchivo(ruta){
    let fs = require('fs')
    let archivo = fs.readFileSync(ruta, 'utf-8')
    return archivo;
}

/**
 * RUTA INICIO
 */
Route.on('/').render('arbol')

/**
 * RUTAS ARBOL
 */
Route.on('/arbol').render('arbol')

Route.get("/getTreeView",async ({ view }) => {
    var contenido = ObtenerTextoArchivo("/proc/201513700_arbol")
    return await contenido;
});

Route.get("/getProcess",async ({ view }) => {
    var contenido = ObtenerTextoArchivo("/proc/201513700_cpu")
    return await contenido;
});

/**
 * RUTAS PROCESOS
 */
Route.get('/killProcess/:id',({ params }) => {
    const proceso = params.id;
    var exec = require('child_process').exec;
    exec('kill '+proceso, function(err, stdout, stderr){
        console.log(stdout);
    });
	return  "LISTO";
})

Route.on('/procesos').render('procesos')

/**
 * RUTAS RAM 
 */
Route.get("/getTotalRam",async ({ view }) => {
    var contenido = ObtenerTextoArchivo("/proc/201513700_ram");
    var arreglo = contenido.split("\n");
    var arreglo1 = arreglo[0].split(":");
    return await  arreglo1[1];
});

Route.get("/getFreeRam",async ({ view }) => {
    var contenido = ObtenerTextoArchivo("/proc/201513700_ram")
    var arreglo = contenido.split("\n");
    var arreglo1 = arreglo[1].split(":");
    return await  arreglo1[1];
});

Route.get("/getUsedRam",async ({ view }) => {
    var contenido = ObtenerTextoArchivo("/proc/201513700_ram")
    var arreglo = contenido.split("\n");
    var arreglo1 = arreglo[2].split(":");
    return await  arreglo1[1];
});


Route.get("/getPercentaje",async ({ view }) => {
    var contenido = ObtenerTextoArchivo("/proc/201513700_ram")
    var arreglo = contenido.split("\n");
    var arreglo1 = arreglo[3].split(":");
    return await  arreglo1[1];
});


Route.on('/ram').render('ram')

/**
 * RUTAS CPU
 */
Route.on('/cpu').render('cpu')

Route.get("/getCPU",async ({ view }) => {
    var uso = cpu.usage()
    .then(cpuPercentage => {
        return cpuPercentage;
    })
	return await  uso ;
});