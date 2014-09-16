VERSION=${FLEETINGPM_RELEASE_VERSION?"is not set."}
DEBIAN_VERSION=$VERSION-1
NAME=fleetingpm
rm -rf *${VERSION}
cp ../sourceforge-git/$NAME-code/$NAME-$VERSION.tar.gz .
mv $NAME-$VERSION.tar.gz ${NAME}_${VERSION}.orig.tar.gz
tar xzvf ${NAME}_${VERSION}.orig.tar.gz
cd $NAME-$VERSION
cp -rv packaging/debian .
debuild -S -sa && cd .. && dput ppa:jussi-lind/ppa ${NAME}_${DEBIAN_VERSION}_source.changes

