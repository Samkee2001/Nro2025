% Uvoz podatkov iz datotek
podatki = readmatrix("vozlisca_temperature_dn2.txt", "NumHeaderLines",4);
x_koordinate = podatki(:,1);
y_koordinate = podatki(:,2);
temperature = podatki(:,3);

celice_podatki = readmatrix("celice_dn2.txt", "NumHeaderLines",2);

% Definicija točke, za katero iščemo temperaturo
x_cilj = 0.403;
y_cilj = 0.503;

% Metoda 1: uporaba scatteredInterpolant za interpolacijo
tic;
interp1 = scatteredInterpolant(x_koordinate, y_koordinate, temperature, "linear");
temperatura1 = interp1(x_cilj, y_cilj);
cas_metoda1 = toc;

% Metoda 2: uporaba griddedInterpolant za interpolacijo
x_unikatne = unique(x_koordinate);
y_unikatne = unique(y_koordinate);
[X_matrika, Y_matrika] = ndgrid(x_unikatne, y_unikatne);
temperaturna_matrika = reshape(temperature, length(x_unikatne), length(y_unikatne));
tic;
interp2 = griddedInterpolant(X_matrika, Y_matrika, temperaturna_matrika, "linear");
temperatura2 = interp2(x_cilj, y_cilj);
cas_metoda2 = toc;

% Metoda 3: lastna implementacija bilinearne interpolacije
tic; 
celica_najdena = false; 

for i = 1:size(celice_podatki, 1)
    % Pridobimo indeksne vrednosti celice
    indeks1 = celice_podatki(i, 1);
    indeks2 = celice_podatki(i, 2);
    indeks3 = celice_podatki(i, 3);
    indeks4 = celice_podatki(i, 4);

    % Pridobimo koordinate in temperature vogalov celice
    x1 = x_koordinate(indeks1);
    y1 = y_koordinate(indeks1);
    T11 = temperature(indeks1);

    x2 = x_koordinate(indeks2);
    y2 = y_koordinate(indeks2);
    T21 = temperature(indeks2);

    x3 = x_koordinate(indeks3);
    y3 = y_koordinate(indeks3);
    T22 = temperature(indeks3);

    x4 = x_koordinate(indeks4);
    y4 = y_koordinate(indeks4);
    T12 = temperature(indeks4);

    % Preverimo, če je točka znotraj trenutne celice
    if x_cilj >= x1 && x_cilj <= x2 && y_cilj >= y1 && y_cilj <= y3
        % Interpolacija po x-osi
        interpolacija_x1 = (x2 - x_cilj) / (x2 - x1) * T11 + (x_cilj - x1) / (x2 - x1) * T21;
        interpolacija_x2 = (x2 - x_cilj) / (x2 - x1) * T12 + (x_cilj - x1) / (x2 - x1) * T22;

        % Interpolacija po y-osi
        temperatura3 = (y3 - y_cilj) / (y3 - y1) * interpolacija_x1 + (y_cilj - y1) / (y3 - y1) * interpolacija_x2;
        break;
    end
end

cas_metoda3 = toc;

% Največja temperatura
temperatura_max = max(temperature);
indeks_max = find(temperature == temperatura_max);
x_max_temp = x_koordinate(indeks_max);
y_max_temp = y_koordinate(indeks_max);

% Izpis rezultatov 
fprintf('Najvišja temperatura je: %.2f\n', temperatura_max);
fprintf('Koordinate vozlišča z najvišjo temperaturo: (%.3f, %.3f)\n', x_max_temp, y_max_temp);
fprintf('Bilinearna interpolacija: %.6f (čas: %.6f s)\n', temperatura3, cas_metoda3);
fprintf('Interpolacija z scatteredInterpolant: %.6f (čas: %.6f s)\n', temperatura1, cas_metoda1);
fprintf('Interpolacija z griddedInterpolant: %.6f (čas: %.6f s)\n', temperatura2, cas_metoda2);
