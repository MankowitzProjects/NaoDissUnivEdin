function out = sprintf2(fmt, varargin)
%SPRINTF2 Quasi-"vectorized" sprintf
%
% out = sprintf2(fmt, varargin)
%
% Like sprintf, but takes arrays of arguments and returns cellstr. This
% lets you do formatted output on nonscalar arrays.
%
% Example:
% food = {'wine','cheese','fancy bread'};
% price = [10 6.38 8.5];
% sprintf2('%-12s %6.2f', food, price)
% % Fancier formatting with width detection
% sprintf2('%-*s %6.2f', max(cellfun('prodofsize',food)), food, price)

[args,n] = promote(varargin);
out = cell(n,1);
for i = 1:n
    argsi = grab(args, i);
    out{i} = sprintf(fmt, argsi{:});
end

% Convenience HACK for display to command line
if nargout == 0
    disp(char(out));
    clear out;
end

function [args,n] = promote(args)
%PROMOTE Munge inputs to get cellstrs
for i = 1:numel(args)
    if ischar(args{i})
        args{i} = cellstr(args{i});
    end
end
n = cellfun('prodofsize', args);
if numel(unique(n(n > 1))) > 1
    error('Inconsistent lengths in nonscalar inputs');
end
n = max(n);

function out = grab(args, k)
%GRAB Get the kth element of each arg, popping out cells
for i = 1:numel(args)
    if isscalar(args{i})
        % "Scalar expansion" case
        if iscell(args{i})
            out{i} = args{i}{1};
        else
            out{i} = args{i};
        end
    else
        % General case - kth element of array
        if iscell(args{i})
            out{i} = args{i}{k};
        else
            out{i} = args{i}(k);
        end
    end
end
