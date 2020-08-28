function [D] = ue_speaker()
    %UNTITLED2 Summary of this function goes here
    %   Detailed explanation goes here
    
    radius = 1; %meter
    
    S = ambi_spkr_array(...
        'ue_speaker', ...
        'AER', 'DDM', ...
        'TFL', [30, 30, 2], ...
        'TFR', [-30, 30, 2], ...
        'TRL', [135, 30, 2], ...
        'TRR', [-135, 30, 2], ...
        'BFL', [30, -30, 2], ...
        'BFR', [-30, -30, 2], ...
        'BRL', [135, -30, 2], ...
        'BRR', [-135, -30, 2], ...
        'T', [0, 90, 2]);
        
    C = ambi_channel_definitions(4, 4, 'HV', 'ACN', 'N3D');
    
    D = ambi_run_allrad(S, C);
    
end

