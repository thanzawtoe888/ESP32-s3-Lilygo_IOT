function [sp_amp, dtmf] = dtmf_detect(data, fs, N) %#codegen
    arguments
        data(1,1600) int32;
        fs(1,1) int32;
        N(1,1) int32;
    end
    sdata = single(data);
    f = complex(zeros(1,N), zeros(1,N));
    f = fft(sdata,N);
    sp = single(zeros(1,N));
    sp = single(abs(f(1:N/2)));
    sp_amp = zeros(1,2);
    sp_amp(1) = mean(sp);
    sp_amp(2) = max(sp);
    [~,locs] = findpeaks(sp, 'NPeaks',2, 'SortStr', 'descend');
    dtmf = zeros(1,2);
    dtmf(1) = locs(1) * double(fs/N);
    dtmf(2) = locs(2) * double(fs/N);
end