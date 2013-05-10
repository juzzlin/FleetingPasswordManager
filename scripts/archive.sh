VERSION=$(git log | grep -m 1 "^\s*[0-9]\.[0-9]\(\.[0-9]\)*" | awk '{print $1}')
git archive --format=tar --prefix=fleetingpm-$VERSION/ HEAD | gzip > fleetingpm-$VERSION.tar.gz
git archive --format=zip --prefix=fleetingpm-$VERSION/ HEAD > fleetingpm-$VERSION.zip

