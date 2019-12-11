/**
Insercion de datos en la base de datos de baloncesto

Se crean:
    4 equipos con 10 jugadores cada uno
    10 partidos
    Resultados de los partidos con solo un equipo invicto
*/

/*Insercion de equipos*/
INSERT INTO Equipos (codE, nombreE, localidad, entrenador, fecha_crea) VALUES
    ("GRD", "Granada Club", "Granada", "Juan Jose Herrera", DATE("now")),
    ("MLG", "Los malagueños", "Malaga", "Antonio Merino Gallardo", DATE("now")),
    ("JAN", "Olivos Club", "Jaen", "Pedro Enrique Garcia", DATE("now")),
    ("CDZ", "Gaditanos", "Cadiz", "Luis Enrique Gomez", DATE("now"));

/*Insercion de jugadores en los equipos*/
INSERT INTO Jugadores (codE, codJ, nombreJ) VALUES
    /*Jugadores de Granada*/
    ("GRD", "PLY1", "Sergio Quijano Rey"),
    ("GRD", "PLY2", "Pablo Ruiz Bueno"),
    ("GRD", "PLY3", "Laura Ruiz"),
    ("GRD", "PLY4", "Lucia Salamanca"),
    ("GRD", "PLY5", "Adrian Lopez"),
    /*Jugadores de Malaga*/
    ("MLG", "PLY6", "Manuel Lopez"),
    ("MLG", "PLY7", "Rosa Parks"),
    ("MLG", "PLY8", "James Harden"),
    ("MLG", "PLY9", "Tomas Lee"),
    ("MLG", "PLY10", "Thomas Shelby"),
    /*Jugadores de Jaen*/
    ("JAN", "PLY11", "Pau Gasol"),
    ("JAN", "PLY12", "Mark Gasol"),
    ("JAN", "PLY13", "Soraya Silvia"),
    ("JAN", "PLY14", "Silvia Soraya"),
    ("JAN", "PLY15", "Mario Cagigas"),
    /*Jugadores de Cadiz*/
    ("CDZ", "PLY16", "Luis Rodriguez"),
    ("CDZ", "PLY17", "Luis Tormo"),
    ("CDZ", "PLY18", "Luis Barcenas"),
    ("CDZ", "PLY19", "Francisco Garcia"),
    ("CDZ", "PLY20", "Diego Mantilla");

/*Insercion de los encuentros
  Granada queda invicta
*/
INSERT INTO Encuentros (ELocal, EVisitante, fecha, Plocal, PVisitante) VALUES
    ("GRD", "MLG", DATE("now"), 100, 80),
    ("MLG", "JAN", DATE("now") + 1, 90, 70),
    ("JAN", "CDZ", DATE("now") + 2, 100, 80),
    ("CDZ", "GRD", DATE("now") + 3, 80, 100),


    ("GRD", "JAN", DATE("now") + 4, 100, 80),
    ("GRD", "CDZ", DATE("now") + 2, 100, 80),

    ("MLG", "GRD", DATE("now") + 3, 20, 80),
    ("MLG", "CDZ", DATE("now") - 1, 100, 80),


    ("CDZ", "JAN", DATE("now"), 100, 80),
    ("CDZ", "MLG", DATE("now"), 70, 80);
