%naloga_1

filename = 'naloga1_1.txt';
podatki = importdata(filename);
delimiterIn="";
headerlinesIn = 2;
data = importdata("naloga1_1.txt",delimiterIn, headerlinesIn);
t = data.data();

%naloga_2
fid = fopen("naloga1_2.txt");
vrstica_1 = fgetl(fid);
razdelitev_vrstice = strsplit(vrstica_1, ": ");
stevilcni_del = razdelitev_vrstice(2);
stevilo_vrstic = str2double(stevilcni_del);

P = zeros(0, stevilo_vrstic);
for i = 1:1:stevilo_vrstic
    vrstica = fgetl(fid);
    P(i) = str2double(vrstica);
end

%Graf
plot(t, P)
title ("Graf P(t)")
xlabel("t[s]")
ylabel("P[W]")

%Naloga_3
rezultat_integrala = 0;
for i = 1:1:(stevilo_vrstic-1)
    dt = t(i+1) - t(i);
    povrsina_trapeza = dt/2 * (P(i) + P(i+1));
    rezultat_integrala = rezultat_integrala + povrsina_trapeza;
end

rezultat_builtin_funkcija = trapz(t, P);

razlika = rezultat_integrala - rezultat_builtin_funkcija
rezultat_builtin_funkcija