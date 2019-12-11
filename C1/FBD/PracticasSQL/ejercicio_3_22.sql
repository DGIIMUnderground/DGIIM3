/**
Enunciado:
    - Encontrar el codigo de las piezas cuyo peso es mayor que el peso de 
    cualquier tornillo
 */

SELECT codpie FROM pieza WHERE peso > ALL (
    SELECT peso FROM pieza WHERE nompie == "Tornillo"
);
